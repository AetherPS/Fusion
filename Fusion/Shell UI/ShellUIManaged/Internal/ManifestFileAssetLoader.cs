using Sce.PlayStation.PUI;
using System;
using System.IO;
using System.Reflection;

namespace Fusion.Internal
{
    internal class ManifestFileAssetLoader : AssetLoader
    {
        private static string GetManifestPath(string uri)
        {
            if (uri.StartsWith("manifest://"))
            {
                return uri.Substring(11).Replace("/", ".");
            }
            return uri.StartsWith("manifest:")
                ? uri.Substring(9).Replace("/", ".")
                : uri.Replace("/", ".");
        }

        public override MasterAsyncToken LoadImageAsync(string uri, ImageOptions option)
        {
            var imageLoadRequest = new ImageLoadRequest
            {
                Uri = uri,
                Option = option,
                ManifestPath = "Fusion.Settings." + GetManifestPath(uri)
            };

            MasterAsyncToken token;
            ThreadPool.Request(
                imageLoadRequest.Load,
                imageLoadRequest.Finish,
                out token
            );

            imageLoadRequest.Token = token;
            return token;
        }

        private class ImageLoadRequest : ImageLoadFinishAction
        {
            public string ManifestPath { get; set; }
            public MasterAsyncToken Token { get; set; }

            public bool Load()
            {
                try
                {
                    var manifestStream = Assembly.GetExecutingAssembly()
                        .GetManifestResourceStream(ManifestPath);

                    if (manifestStream is null)
                    {
                        return false;
                    }

                    using (var binaryReader = new BinaryReader(manifestStream))
                    {
                        byte[] imageData = binaryReader.ReadBytes((int)manifestStream.Length);
                        Image = UIImage.Create(imageData, Option);
                    }

                    // Create texture on the background thread before returning to main thread
                    // This is inherited from ImageLoadFinishAction
                    Reflect.Call(this, "CreateTextureIfNeeded");

                    return Image != null;
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Failed to load manifest resource: {ManifestPath}");
                    Console.WriteLine(ex.ToString());
                    return false;
                }
            }

            protected override void NotifyComplete(AsyncCompletedEventArgs args, AssetObject obj)
            {
                if (obj != null)
                {
                    Token?.NotifyComplete(args, obj);
                }
                else
                {
                    Token?.NotifyComplete(args);
                }
            }
        }
    }
}
