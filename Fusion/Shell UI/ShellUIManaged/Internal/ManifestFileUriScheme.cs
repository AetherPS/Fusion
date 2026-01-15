using Sce.PlayStation.PUI;

namespace Fusion.Internal
{
    public static class ManifestFileUriScheme
    {
        public static void Initialize()
        {
            if (_assetLoader is null)
            {
                _assetLoader = new ManifestFileAssetLoader();
                AssetLoader.Register("manifest", _assetLoader);
            }
        }

        public static void Terminate()
        {
            _assetLoader?.Terminate();
        }

        private static ManifestFileAssetLoader _assetLoader;
    }
}
