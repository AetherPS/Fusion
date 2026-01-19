using Fusion.Internal;
using Sce.PlayStation.Core.Imaging;
using Sce.PlayStation.PUI;
using Sce.PlayStation.PUI.UI2;
using Sce.Vsh.Lx;
using Sce.Vsh.ShellUI.AppSystem;
using Sce.Vsh.ShellUI.Library;
using Sce.Vsh.ShellUI.TopMenu;
using System;

namespace Fusion.Features.Devkit
{
    public static class DevkitTitleIdLabel
    {
        private unsafe delegate void ContentDecoratorBase_ctorDelegate(IntPtr instance, IntPtr param);
        private static ContentDecoratorBase_ctorDelegate _ContentDecoratorBase_ctor_stub;
        private static bool _showLabels = false;

        public static bool ShowLabels
        {
            get => _showLabels;
            set
            {
                if (_showLabels != value)
                {
                    _showLabels = value;
                    if (_showLabels)
                        ShowAllLabels();
                    else
                        HideAllLabels();
                }
            }
        }

        [MethodOverride(typeof(ContentDecoratorBase), ".ctor")]
        public static unsafe void ContentDecoratorBase_ctor(ContentDecoratorBase instance, ContentDecoratorParam param)
        {
            _ContentDecoratorBase_ctor_stub(*(IntPtr*)&instance, *(IntPtr*)&param);
            if (_showLabels)
            {
                CreateLabel(instance);
            }
        }

        private static void CreateLabel(ContentDecoratorBase instance)
        {
            try
            {
                var iconImageBox = Reflect.Get<ImageBox>(instance, "m_iconImageBox");
                if (iconImageBox == null)
                    return;

                string titleId = instance.AppBrowseItem.GetTitleId();

                Label label = iconImageBox.Append(new Label
                {
                    Font = new UIFont(UIFont.SizeXXSmall, FontStyle.Italic, FontWeight.Medium),
                    FitHeightToText = true,
                    LayoutRule = new Anchor(Anchors.Left | Anchors.Right)
                    {
                        Left = 4f,
                        Right = 4f
                    },
                    EnableThemedColor = false,
                    EnableThemedShadowColor = false,
                    TextColor = new UIColor(1f, 1f, 1f),
                    TextShadow = new TextShadowSettings
                    {
                        Color = new UIColor(0f, 0f, 0f)
                    }
                });

                label.FontConfig.LargeFontEnabled = false;
                label.FontConfig.BoldFontEnabled = false;
                label.Text = titleId;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[DebugTitleIdLabel] CreateLabel error: {ex.Message}");
            }
        }

        private static void RemoveLabel(ContentDecoratorBase instance)
        {
            try
            {
                var iconImageBox = Reflect.Get<ImageBox>(instance, "m_iconImageBox");
                if (iconImageBox == null)
                    return;

                var children = iconImageBox.GetChildrenArray();
                if (children == null)
                    return;

                foreach (var child in children)
                {
                    if (child is Label)
                        ((Widget)child).RemoveFromParent();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[DebugTitleIdLabel] RemoveLabel error: {ex.Message}");
            }
        }

        private static void ForEachDecorator(Action<ContentDecoratorBase> action)
        {
            try
            {
                var scene = Reflect.Get<object>(ContentsAreaManager.Instance, "m_scene");
                var gridList = Reflect.Get<GridListPanel[]>(scene, "m_contentsGridList");

                foreach (var grid in gridList)
                {
                    if (grid == null)
                        continue;

                    foreach (ListPanelItem listPanelItem in grid.ActiveItems)
                    {
                        var listItem = (ListItem)listPanelItem;
                        var contentVisualizer = listItem.ListVisualizer as ContentVisualizer;
                        var decorator = contentVisualizer?.GetDecorator();

                        if (decorator != null)
                            action(decorator);
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[DebugTitleIdLabel] ForEachDecorator error: {ex.Message}");
            }
        }

        private static void ShowAllLabels() => ForEachDecorator(CreateLabel);

        private static void HideAllLabels() => ForEachDecorator(RemoveLabel);
    }
}