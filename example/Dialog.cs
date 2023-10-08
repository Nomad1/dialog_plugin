using System;

namespace Godot.Plugins
{
    /// <summary>
    /// This class is a front-end for multiplatform plugin for system dialog
    /// </summary>
    public class Dialog : Object
    {
        #region Static

        private static readonly Dialog s_instance = new Dialog();

        /// <summary>
        /// Show the system dialog window with specified title, message and buttons. When one of buttons is clicked, callback function is
        /// called with the button id
        /// </summary>
        /// <param name="title">Window title</param>
        /// <param name="message">Window message</param>
        /// <param name="buttons">List of buttons. If null or empty, there will be only one "Ok" button</param>
        /// <param name="callback">Callback with index of pressed button</param>
        /// <returns>Id of the dialog shown for precise Hide calls </returns>
        public static int Show(string title, string message, string[] buttons = null, Action<int> callback = null)
        {
            if (s_instance == null)
            {
                Console.Error.WriteLine("Dialog plugin is not installed!");
                return -1;
            }

            return s_instance.DoShow(title, message, buttons, callback);
        }

        /// <summary>
        /// Closes the dialog window. Sometimes this method could be called too late and to avoid closing current dialog it could check the id
        /// </summary>
        public static void Hide(int id = -1)
        {
            if (s_instance == null)
            {
                Console.Error.WriteLine("Dialog plugin is not installed!");
                return;
            }

            s_instance.DoHide(id);
        }

        #endregion

        private readonly Godot.Object m_dialogPlugin;

        private readonly System.Collections.Generic.Dictionary<int, Action<int>> m_dialogs = new System.Collections.Generic.Dictionary<int, Action<int>>();

        private Dialog()
        {
            try
            {
                m_dialogPlugin = Godot.Engine.GetSingleton("Dialog");

                if (m_dialogPlugin == null)
                {
                    NativeScript ns = GD.Load<NativeScript>("res://plugins/dialog/dialog.gdns");
                    m_dialogPlugin = (Object)ns.New();

                    Console.WriteLine("Loaded {0}, class {1}", m_dialogPlugin, m_dialogPlugin.GetClass());
                }

                if (m_dialogPlugin == null)
                {
                    Console.Error.WriteLine("Error getting Dialog plugin, make sure plugin is installed!");
                }
                else
                {
#if GODOT4
                    m_dialogPlugin.Connect("dialog_closed", Callable.From(OnClosed)));
#else
                    m_dialogPlugin.Connect("dialog_closed", this, "OnClosed");
#endif

                    Console.WriteLine("Dialog singleton inited");
                }
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("Error initing Dialog plugin: {0}", ex);
            }
        }

        public void OnClosed(int id, int index)
        {
            Console.WriteLine("Dialog result: {0} for {1}", index, id);

            if (m_dialogs.ContainsKey(id))
                ProcessResult(id, index);
        }

        private int DoShow(string title, string message, string[] buttons, Action<int> callback)
        {
            if (m_dialogPlugin == null)
            {
                Console.Error.WriteLine("Dialog plugin is not installed, fallback to OS.alert()!");

                // Theoretically we don't need this part since native plugins use OS.alert() for non-supported platforms,
                // however I'm keeping this code just in case there are no native libraries at all.
                Godot.OS.Alert(message, title);

                if (callback != null)
                    callback(0);
                return -1;
            }

            Godot.Collections.Array buttonArray;

            if (buttons == null)
                buttonArray = new Godot.Collections.Array();
            else
                buttonArray = new Godot.Collections.Array(buttons);

            int dialogId = (int)s_instance.m_dialogPlugin.Call("show", title, message, buttonArray);

            m_dialogs[dialogId] = callback; // add to dictionary even if callback is null

            return dialogId;
        }

        private void DoHide(int id)
        {
            if (m_dialogPlugin == null)
            {
                Console.Error.WriteLine("Dialog plugin is not installed!");
                return;
            }

            if (id != -1)
            {
                if (!m_dialogs.ContainsKey(id))
                {
                    Console.Error.WriteLine("Called Hide when dialog for unknown dialog {0}!", id);
                    return;
                }
            }

            s_instance.m_dialogPlugin.Call("hide", id);

            if (id == -1) // hide eveything
            {
                int[] keys = new int[m_dialogs.Count];
                m_dialogs.Keys.CopyTo(keys, 0);

                foreach (int nid in keys)
                    ProcessResult(nid, -1);
            }
            else
            {
                ProcessResult(id, -1);
            }
        }

        private void ProcessResult(int id, int result)
        {
            Action<int> callback;

            if (m_dialogs.TryGetValue(id, out callback))
                m_dialogs.Remove(id);

            if (callback != null)
                callback(result);
        }
    }
}

