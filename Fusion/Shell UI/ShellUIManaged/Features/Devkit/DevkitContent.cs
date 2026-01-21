using Fusion.Internal;
using Sce.PlayStation.PUI;
using Sce.Vsh.Accessor;
using Sce.Vsh.Accessor.Db;
using Sce.Vsh.Lx;
using Sce.Vsh.ShellUI.AppSystem;
using Sce.Vsh.ShellUI.Base;
using Sce.Vsh.ShellUI.Library;
using Sce.Vsh.ShellUI.TopMenu;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

namespace Fusion.Features.Devkit
{
    internal static class DevkitContent
    {
        private unsafe delegate int ExecuteCountQueryDelegate(IntPtr instance);
        private unsafe delegate IntPtr ExecuteSelectQueryDelegate(IntPtr instance, int offset, int limit);
        private unsafe delegate IntPtr GetIconPathDelegate(IntPtr item, bool withTheme);
        private unsafe delegate void LoadFocusInfoDelegate(IntPtr instance);

        private static ExecuteCountQueryDelegate _ExecuteCountQuery_stub;
        private static ExecuteSelectQueryDelegate _ExecuteSelectQuery_stub;
        private static ExecuteSelectQueryDelegate _ExecuteSelectQueryForIndex_stub;
        private static GetIconPathDelegate _GetIconPath_stub;
        private static LoadFocusInfoDelegate _LoadFocusInfo_stub;

        private static IList<AppBrowseItem> MemoryItemList
        {
            get
            {
                List<AppBrowseItem> list = new List<AppBrowseItem>();

                if (_appHome)
                {
                    list.Add(new AppBrowseItem
                    {
                        TitleId = "NPXS29999",
                        TitleName = "★APP_HOME(host)",
                        MetaDataPath = string.Empty
                    });
                    list.Add(new AppBrowseItem
                    {
                        TitleId = "NPXS29998",
                        TitleName = "★APP_HOME(data)",
                        MetaDataPath = string.Empty
                    });
                }

                if (_debugSettingsShortcut)
                {
                    list.Add(new AppBrowseItem
                    {
                        TitleId = "NPXS20993",
                        TitleName = "★Debug Settings",
                        MetaDataPath = string.Empty
                    });
                }

                return list;
            }
        }

        private static bool _debugSettingsShortcut = false;
        public static bool DebugSettingsShortcut
        {
            get
            {
                return _debugSettingsShortcut;
            }

            set
            {
                if (_debugSettingsShortcut != value)
                {
                    _debugSettingsShortcut = value;
                    RefreshTopMenu();
                }
            }
        }

        private static bool _appHome = false;
        public static bool AppHome
        {
            get
            {
                return _appHome;
            }

            set
            {
                if (_appHome != value)
                {
                    _appHome = value;
                    RefreshTopMenu();
                }
            }
        }

        public static void RefreshTopMenu()
        {
            ContentAreaScene scene = (ContentAreaScene)AreaManager.Instance.GetContainerScene(AreaType.Contents).Find("ContentAreaScene");
            scene.m_contentsList.First().ReloadItemSource();
        }

        private static List<AppBrowseItem> GetMemoryItemList(ref int offset, ref int limit)
        {
            var list = new List<AppBrowseItem>();

            try
            {
                int num = offset;
                int num2 = limit;
                int num3;
                if (offset + limit <= MemoryItemList.Count)
                {
                    num3 = limit;
                    num = 0;
                    num2 = 0;
                }
                else if (offset < MemoryItemList.Count)
                {
                    num3 = MemoryItemList.Count - offset;
                    num = 0;
                    num2 = limit - num3;
                }
                else
                {
                    num3 = 0;
                    num = offset - MemoryItemList.Count;
                    num2 = limit;
                }
                list = new List<AppBrowseItem>();
                for (int i = offset + num3 - 1; i >= offset; i--)
                {
                    list.Insert(0, MemoryItemList[i]);
                }
                offset = num;
                limit = num2;
            }
            catch
            {
                
            }

            return list;
        }

        [MethodOverride(typeof(AppBrowseItemAccessor))]
        public static unsafe int ExecuteCountQuery(AppBrowseItemAccessor instance)
        {
            int count = _ExecuteCountQuery_stub(*(IntPtr*)&instance);

            if (Reflect.Get<AppBrowseItemAccessor.FilterTypeAppHome>(instance, "exclusionFilterTypeAppHome") != AppBrowseItemAccessor.FilterTypeAppHome.None)
                return count;
            
            return count + MemoryItemList.Count;
        }

        [MethodOverride(typeof(AppBrowseItemAccessor))]
        public static unsafe List<Item> ExecuteSelectQuery(AppBrowseItemAccessor instance, int offset, int limit)
        {
            IntPtr resultPtr = _ExecuteSelectQuery_stub(*(IntPtr*)&instance, offset, limit);
            List<Item> result = *(List<Item>*)&resultPtr;

            if (Reflect.Get<AppBrowseItemAccessor.FilterTypeAppHome>(instance, "exclusionFilterTypeAppHome") != AppBrowseItemAccessor.FilterTypeAppHome.None)
                return result;

            List<Item> list = new List<Item>();

            List<AppBrowseItem> memoryItemList = GetMemoryItemList(ref offset, ref limit);
            if (memoryItemList != null)
            {
                foreach (AppBrowseItem appBrowseItem in memoryItemList)
                {
                    list.Add(new AppBrowseItem
                    {
                        TitleId = appBrowseItem.TitleId,
                        TitleName = appBrowseItem.TitleName,
                        MetaDataPath = appBrowseItem.MetaDataPath
                    });
                }
            }

            return list.Concat(result).ToList();
        }

        [MethodOverride(typeof(AppBrowseItemAccessor))]
        public static unsafe List<Item> ExecuteSelectQueryForIndex(AppBrowseItemAccessor instance, int offset, int limit)
        {
            IntPtr resultPtr = _ExecuteSelectQueryForIndex_stub(*(IntPtr*)&instance, offset, limit);
            List<Item> result = *(List<Item>*)&resultPtr;

            if (Reflect.Get<AppBrowseItemAccessor.FilterTypeAppHome>(instance, "exclusionFilterTypeAppHome") != AppBrowseItemAccessor.FilterTypeAppHome.None)
                return result;

            List<Item> list = new List<Item>();

            List<AppBrowseItem> memoryItemList = GetMemoryItemList(ref offset, ref limit);
            if (memoryItemList != null)
            {
                foreach (AppBrowseItem appBrowseItem in memoryItemList)
                {
                    list.Add(new AppBrowseItemLite
                    {
                        TitleId = appBrowseItem.TitleId
                    });
                }
            }

            return list.Concat(result).ToList();
        }

        [MethodOverride(typeof(AppBrowseItemMethodExteneder))]
        public static unsafe string GetIconPath(this AppBrowseItem item, bool withTheme = false)
        {
            string titleId = item.GetTitleId();
            if (titleId == "NPXS29999")
            {
                return BasePlugin.GetTexture("tex_app_home");
            }
            if (titleId == "NPXS29998")
            {
                return BasePlugin.GetTexture("tex_app_home_data");
            }
            if (titleId == "NPXS20993")
            {
                return BasePlugin.GetTexture("tex_debug_settings");
            }

            IntPtr resultPtr = _GetIconPath_stub(*(IntPtr*)&item, withTheme);
            return *(string*)&resultPtr;
        }

        //[MethodOverride(typeof(ContentsList))]
        //private static unsafe void LoadFocusInfo(ContentsList instance)
        //{
        //    _LoadFocusInfo_stub(*(IntPtr*)&instance);
            
        //    if (instance.FolderAppBrowseItem == null)
        //    {
        //        instance.SetDefaultFocusIndex(MemoryItemList.Count + 1);
        //    }
        //}

        //[MethodOverride(typeof(ContentsList))]
        //public static bool SetFocusToHome(ContentsList instance)
        //{
        //    int val = MemoryItemList.Count + 2;
        //    if (0 < instance.GridListPanel.ItemCount)
        //    {
        //        int num = Math.Min(val, instance.GridListPanel.ItemCount - 1);
        //        if (instance.GridListPanel.FocusIndex != num)
        //        {
        //            instance.SetFocusIndex(num, true, false);
        //            return true;
        //        }
        //    }
        //    return false;
        //}

        //[MethodOverride(typeof(ContentsList))]
        //private static void DoReorder(ContentsList instance, long delayLimit = 0L)
        //{
        //    if (instance.Disposed || ThemePreview.Enabled || instance.FolderAppBrowseItem != null)
        //    {
        //        return;
        //    }

        //    if (Reflect.Get<bool>(instance, "m_reorderBlocked"))
        //    {
        //        Reflect.Set(instance, "m_reorderDirty", true);
        //        return;
        //    }

        //    string titleId = ContentAreaScene.GetLayerFocusTarget(LayerManager.GetFocusLayer());
        //    if (titleId.Empty())
        //    {
        //        return;
        //    }

        //    RefObj<CachedItemAccessor> accessorReference = instance.GetAppBrowseItemAccessorReference();
        //    if (accessorReference == null)
        //    {
        //        return;
        //    }

        //    AppBrowseItemAccessor accessor = accessorReference.Body.accessor as AppBrowseItemAccessor;

        //    int userId = Reflect.GetProp<int>(typeof(TopMenuPlugin), "UserId");
        //    Reflect.Call(instance, "PostJob", new object[] 
        //    {
        //        1,
        //        UT.Enqueue(ListViewManager.GetFastJobQueue(),
        //        delegate (Job job)
        //        {
        //            AppBrowseItem itemByTitleId = AppBrowseItemMethodExteneder.GetItemByTitleId(userId, titleId);
        //            if (itemByTitleId != null)
        //            {
        //                bool flag = false;
        //                string text = "";
        //                if (itemByTitleId.IsVisibleTvAndVideoItem() && !itemByTitleId.IsVisibleContentAreaItem())
        //                {
        //                    flag = true;
        //                    titleId = TvItemManager.GetTvItemStatus().GetAttachedTitleId(titleId);
        //                    if (!AppBrowseItemMethodExteneder.GetItemByTitleId(userId, titleId).IsVisibleContentAreaItem())
        //                    {
        //                        return;
        //                    }
        //                }
        //                else if (itemByTitleId.IsInFolder())
        //                {
        //                    text = itemByTitleId.GetParentFolderId();
        //                }

        //                int num = MemoryItemList.Count + 2 + 1;
        //                List<Item> items = accessor.GetItems(0, num);
        //                if (items.Count >= num)
        //                {
        //                    AppBrowseItem item = items[num - 1] as AppBrowseItem;
        //                    if (!(item.GetTitleId() == titleId) && (text.Empty() || !(item.GetTitleId() == text)))
        //                    {
        //                        while (UT.ElapsedMilliseconds < delayLimit && !job.IsCancelled)
        //                        {
        //                            Thread.Sleep(1);
        //                        }

        //                        if (!job.IsCancelled)
        //                        {
        //                            if (flag)
        //                            {
        //                                TvItemManager.Push(titleId);
        //                            }
        //                            else
        //                            {
        //                                AppBrowseAccessorWrapper.UpdateLastAccessTime(titleId);
        //                            }
        //                        }
        //                    }
        //                }
        //            }
        //        },
        //        delegate (JobCompletedEventArgs obj)
        //        {
        //            obj.NoThrow();
        //            accessorReference.Dispose();
        //        })
        //    });
        //}
    }
}
