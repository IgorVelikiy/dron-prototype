using Forms3D;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using Xamarin.Forms;
using System.Threading;

namespace ClientSTM
{
    enum Direction : byte
        { Forward = 1, Back, Right, Left, Up, Down, CW, CCW, Error = 255 }
	public partial class MainPage : ContentPage
	{
        private static byte direction = 0;
        private static Stopwatch stopwatch = new Stopwatch();
        private static bool stopAnimation = false;
        private static Type figureType = typeof(Cube);
        private static SharedLineMesh mesh;
        private static SharedLine[] sharedLines;
        private static LineView[] lineViews;
        public MainPage()
        {
            InitializeComponent();
            mesh = (SharedLineMesh)Activator.CreateInstance(figureType);
            sharedLines = mesh.SharedLines.ToArray();
            lineViews = new LineView[sharedLines.Length];

            // Set up page with LineView elements.
            AbsoluteLayout absoluteLayout = new AbsoluteLayout
            {

            };
            for (int i = 0; i < lineViews.Length; i++)
            {
                lineViews[i] = new LineView
                {
                    Thickness = 3
                };
                absoluteLayout.Children.Add(lineViews[i]);
            }

            contentStack.Children.Add(absoluteLayout);

            App.pg = this;
        }
        private void OnButtonPressed(object sender, EventArgs e)
        {
            Button button = (Button)sender;
            switch (button.Text)
            {
                case "Forw.":
                    direction = (byte)Direction.Forward;
                    break;
                case "Back":
                    direction = (byte)Direction.Back;
                    break;
                case "Right":
                    direction = (byte)Direction.Right;
                    break;
                case "Left":
                    direction = (byte)Direction.Left;
                    break;
                case "Up":
                    direction = (byte)Direction.Up;
                    break;
                case "Down":
                    direction = (byte)Direction.Down;
                    break;
                case "+":
                    direction = (byte)Direction.CW;
                    break;
                case "-":
                    direction = (byte)Direction.CCW;
                    break;
                default:
                    direction = (byte)Direction.Error;
                    break;
            }
            App.stream.WriteByte(direction);
        }
        private void OnButtonReleased(object sender, EventArgs e)
        {
            direction = 0;
            App.stream.WriteByte(direction);
        }

        public static void ReadingTcp(object objPage)
        {
            //Thread.Sleep(1000);
            while (true)
            {
                MainPage page = (MainPage)objPage;
                var data = new byte[16];
                while (!App.stream.DataAvailable);
                //Thread.Sleep(50);
                int bytes = App.stream.Read(data, 0, data.Length);
                //if (bytes != 16)
                    //continue;
                var q = new float[4];
                for (int i = 0; i < 16; i += 4)
                    q[i / 4] = BitConverter.ToSingle(data, i);
                Matrix4D rotationMatrix = new Matrix4D(
                    2 * q[0] * q[0] - 1 + 2 * q[1] * q[1], 2 * (q[1] * q[2] + q[0] * q[3]), 2 * (q[1] * q[3] - q[0] * q[2]), 0,
                    2 * (q[1] * q[2] - q[0] * q[3]), 2 * q[0] * q[0] - 1 + 2 * q[2] * q[2], 2 * (q[2] * q[3] + q[0] * q[1]), 0,
                    2 * (q[1] * q[3] + q[0] * q[2]), 2 * (q[2] * q[3] - q[0] * q[1]), 2 * q[0] * q[0] - 1 + 2 * q[3] * q[3], 0,
                    0, 0, 0, 1);
                Matrix4D matView = Matrix4D.OrthographicView(new Point(page.Width / 2, page.Height / 2),
                                                             Math.Min(page.Width, page.Height) / 4);

                // Get composite 3D transform matrix.
                Matrix4D matrix = rotationMatrix * matView;

                // Loop through the shared lines that comprise the figure.
                for (int i = 0; i < sharedLines.Length; i++)
                {
                    int k = i;
                    // Transform the points and normals of those points. 
                    Point3D point1 = sharedLines[i].Point1 * matrix;
                    Point3D point2 = sharedLines[i].Point2 * matrix;
                    Vector3D normal1 = (Point3D)((Point3D)(sharedLines[i].Normal1) * matrix);
                    Vector3D normal2 = (Point3D)((Point3D)(sharedLines[i].Normal2) * matrix);

                    Device.BeginInvokeOnMainThread(() =>
                    {
                        // Set each LineView to its new position.
                        lineViews[k].Point1 = point1;
                        lineViews[k].Point2 = point2;

                        // Determine if the line is visible or hidden.
                        bool isFacing = normal1.Z > 0 || normal2.Z > 0;
                        lineViews[k].Color = isFacing ? Color.Accent : Color.FromRgba(0.75, 0.75, 0.75, 0.5);
                    });
                }
            }
        }
    }
}
