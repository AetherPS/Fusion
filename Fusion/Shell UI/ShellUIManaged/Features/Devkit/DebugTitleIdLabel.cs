using Sce.PlayStation.Core.Imaging;
using Sce.PlayStation.PUI;
using Sce.PlayStation.PUI.UI2;
using Sce.Vsh.Lx;
using Sce.Vsh.ShellUI.AppSystem;
using Sce.Vsh.ShellUI.Library;
using Sce.Vsh.ShellUI.TopMenu;
using System;
using System.Reflection;


namespace Fusion
{
    public static class DebugTitleIdLabel
    {
        private static Detour _constructorDetour;

        private static FieldInfo _iconImageBoxField;
        private static FieldInfo _sceneField;
        private static FieldInfo _gridListField;

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

        public static void Install()
        {
            try
            {
                CacheReflection();
                InstallHooks();
                Console.WriteLine("[DebugTitleIdLabel] Installed");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[DebugTitleIdLabel] Install failed: {ex.Message}");
            }
        }

        private static void CacheReflection()
        {
            var flags = BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public;

            var contentDecoratorBaseType = Type.GetType("Sce.Vsh.ShellUI.Library.ContentDecoratorBase, app");
            _iconImageBoxField = contentDecoratorBaseType.GetField("m_iconImageBox", flags);

            var contentsAreaManagerType = Type.GetType("Sce.Vsh.ShellUI.TopMenu.ContentsAreaManager, app");
            _sceneField = contentsAreaManagerType.GetField("m_scene", flags);

            var contentAreaSceneType = Type.GetType("Sce.Vsh.ShellUI.TopMenu.ContentAreaScene, app");
            _gridListField = contentAreaSceneType.GetField("m_contentsGridList", flags);
        }

        private static void InstallHooks()
        {
            var contentDecoratorBaseType = Type.GetType("Sce.Vsh.ShellUI.Library.ContentDecoratorBase, app");

            var ctorHook = typeof(DebugTitleIdLabel).GetMethod(nameof(Constructor_Hook), BindingFlags.Static | BindingFlags.Public);
            _constructorDetour = Detour.CreateForConstructor(
                contentDecoratorBaseType,
                new[] { typeof(ContentDecoratorParam) },
                ctorHook
            );
        }

        public static void Constructor_Hook(ContentDecoratorBase instance, object param)
        {
            _constructorDetour.CallOriginal(() =>
            {
                var ctor = instance.GetType().GetConstructor(
                    BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.Public,
                    null, new[] { param.GetType() }, null);
                ctor?.Invoke(instance, new[] { param });
            });

            if (_showLabels)
                CreateLabel(instance);
        }

        private static void CreateLabel(ContentDecoratorBase instance)
        {
            try
            {
                ImageBox iconImageBox = _iconImageBoxField.GetValue(instance) as ImageBox;
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
                ImageBox iconImageBox = _iconImageBoxField.GetValue(instance) as ImageBox;
                if (iconImageBox == null)
                    return;

                var children = iconImageBox.GetChildrenArray();
                if (children == null)
                    return;

                foreach (var child in children)
                {
                    if (child is Label)
                    {
                        ((Widget)child).RemoveFromParent();
                    }
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
                var scene = _sceneField.GetValue(ContentsAreaManager.Instance);
                GridListPanel[] gridList = (GridListPanel[])_gridListField.GetValue(scene);

                foreach (GridListPanel grid in gridList)
                {
                    if (grid == null)
                        continue;

                    foreach (ListPanelItem listPanelItem in grid.ActiveItems)
                    {
                        ListItem listItem = (ListItem)listPanelItem;
                        ContentVisualizer contentVisualizer = listItem.ListVisualizer as ContentVisualizer;
                        var decorator = contentVisualizer.GetDecorator();

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

        private static void ShowAllLabels()
        {
            ForEachDecorator(CreateLabel);
        }

        private static void HideAllLabels()
        {
            ForEachDecorator(RemoveLabel);
        }

        public static void Uninstall()
        {
            _constructorDetour?.Dispose();
            _constructorDetour = null;

            Console.WriteLine("[DebugTitleIdLabel] Uninstalled");
        }
    }
}