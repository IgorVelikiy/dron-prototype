using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using Xamarin.Forms;
using System.Net.Sockets;

namespace ClientSTM
{
	public partial class App : Application
	{
        private const string ipESP = "192.168.0.1";
        private const int port = 9876;
        public static NetworkStream stream = null;
        public static MainPage pg = null;
        public App ()
		{
			InitializeComponent();
            //MainPage = new DisplayPage();
            MainPage = new MainPage();
        }

		protected override void OnStart ()
		{
            TcpClient tcpClient = new TcpClient(ipESP, port);
            stream = tcpClient.GetStream();
            new Thread(ClientSTM.MainPage.ReadingTcp).Start(pg);
        }

		protected override void OnSleep ()
		{
			// Handle when your app sleeps
		}

		protected override void OnResume ()
		{
			// Handle when your app resumes
		}


	}
}
