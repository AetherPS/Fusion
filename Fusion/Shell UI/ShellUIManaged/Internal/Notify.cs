using Sce.PlayStation.Json;
using Sce.Vsh.ShellUI.NotificationUtil;
using System;
using System.Collections.Generic;

namespace Fusion.Internal
{
    internal class Notify
    {
        public static void Request(string icon, string message, string message2)
        {
            JsonObject jsonObject = new JsonObject(Array.Empty<KeyValuePair<string, JsonValue>>());
            if (jsonObject != null)
            {
                jsonObject["MsgId"] = 100;
                jsonObject["TargetId"] = -1;
            }

            SystemMsg.Request(new SystemMsg.NotificationEntry()
            {
                type = SystemMsg.EntryType.TypeStandard,
                imageUri = icon,
                message = message,
                message2 = message2,
                userId = -1,
                obj = jsonObject,
            }, 0, 0, Sce.Vsh.ShellUI.Notification.EntryOption.ForceLong);
        }
    }
}
