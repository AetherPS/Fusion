using Sce.PlayStation.Core.Imaging;
using Sce.PlayStation.PUI;
using Sce.PlayStation.PUI.UI2;
using Sce.Vsh.ShellUI.AppSystem;
using Sce.Vsh.ShellUI.TopMenu;
using System;

namespace Fusion.TopMenu
{
    internal class SystemAreaIconFusion : SystemAreaIconBase
    {
        private float m_defaultPosX;

        public SystemAreaIconFusion(Panel w) : base(w, false, null)
        {
        }

        public override void Dispose()
        {
            UnloadSystemAreaPlugin();
            base.Dispose();
        }

        protected override void InitIcon()
        {
            // Find or create the icon image
            m_iconImage = (ImageBox)basePanel.FindChildByName("FusionImage");
            if (m_iconImage == null)
            {
                m_iconImage = new ImageBox
                {
                    Name = "FusionImage",
                    Width = 128,
                    Height = 128,
                    ImageScaleType = ImageScaleType.AspectInside,
                    Opacity = 0f,
                };
                basePanel.AppendChild(m_iconImage);
            }

            // Load your icon - you can replace this URL
            m_iconImage.LoadAsync("manifest://SettingsRoot/data/logo.png", null, new ImageOptions
            {
                AssetManager = new AssetManager("FusionIcon", 4960256),
                Format = ImageFormat.Dxt5,
                ConvertOption = ImageConvertOption.DXTCompressByGPU,
            });

            // Find or create the label
            m_iconLabel = (Label)basePanel.FindChildByName("FusionLabel");
            if (m_iconLabel == null)
            {
                m_iconLabel = new Label
                {
                    Name = "FusionLabel",
                    Font = new UIFont(UIFont.SizeXSmall, FontStyle.Normal, FontWeight.Normal),
                    TextTrimming = TextTrimming.None,
                    LineBreak = LineBreak.Character,
                    TextColor = new UIColor(1, 1, 1),
                    HorizontalAlignment = HorizontalAlignment.Center,
                    VerticalAlignment = VerticalAlignment.Bottom,
                    X = -192,
                    Y = 140,
                    Width = 512,
                    Height = 35,
                    Opacity = 0f,
                };
                basePanel.AppendChild(m_iconLabel);
            }
            m_iconLabel.Text = "Fusion Menu";

            m_defaultPosX = basePanel.X;
        }

        public override void IconOpen(AnimationBlock a)
        {
            m_iconImage.Opacity = 1f;
            base.IconOpen(a);
        }

        public override void IconClose(AnimationBlock a)
        {
            m_iconImage.Opacity = 0f;
            base.IconClose(a);
        }

        public override void AdjustLayout(float width)
        {
            float x = m_defaultPos.X = m_defaultPosX - width;
            if (!m_opened)
            {
                basePanel.X = x;
            }
        }

        public override void IconKeyEventReceived(object sender, KeyEventArgs args)
        {
            bool handled = false;
            if (args.KeyEventType == KeyEventType.Down)
            {
                switch (args.KeyType)
                {
                    case KeyType.Enter:
                        LaunchPlugin();
                        handled = true;
                        break;
                }
            }
            if (handled)
            {
                args.Handled = true;
            }
            base.IconKeyEventReceived(sender, args);
        }

        protected override string GetGlowPath()
        {
            // You can return null or provide a glow texture path
            return null;
        }

        protected override bool EnabledFocusEffect()
        {
            // For now, disable custom glow effect
            return false;
        }

        public static string LaunchUrl
        {
            get
            {
                // This is the URI that will be called when the icon is selected
                return "pssettings:play?mode=settings&function=fusion_menu";
            }
        }

        private void LaunchPlugin()
        {
            Console.WriteLine("[Fusion] Launching Fusion Menu via URI: " + LaunchUrl);
            BootHelper.Boot(LaunchUrl, BootHelper.Option.None, null, null);
        }

        public override void IconSelect()
        {
            LaunchPlugin();
        }
    }
}