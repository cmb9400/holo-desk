using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace Microsoft.Samples.Kinect.DepthBasics
{
    class MouseStreamNet
    {
        private TcpClient client = null;
        private StreamWriter netStream = null;
        private bool enabled;
        private DateTime lastConnectAttempted;
        private string address;
        private int port;

        public MouseStreamNet(bool enable = true, string address = "129.21.207.17", int port = 1313)
        {
            this.enabled = enable;
            if (this.enabled)
            {
                this.address = address;
                this.port = port;
                try
                {
                    connect();
                }
                catch (SocketException se)
                {
                    this.enabled = false;
                    Console.WriteLine("failed to connect to mouse");
                    Console.WriteLine(se.Message);
                }
            }
        }

        public void DataReceived (object sender, DataReceivedEventArgs args)
        {
            Console.WriteLine(args.Data);
            if (!this.enabled) return;
            try
            {
                this.netStream.WriteLine(args.Data);
                this.netStream.Flush();
            } catch (Exception e)
            {
                Console.Error.WriteLine(e.Message);
                if ((DateTime.Now - lastConnectAttempted).Seconds > 3)
                {
                    try
                    {
                        Console.WriteLine("Attempting to reconnect");
                        connect();
                    } catch (SocketException)
                    {
                        Console.WriteLine("Reconnect failed");
                    }
                }
            }
        }


        private void connect()
        {
            if (!this.enabled) return;
            this.lastConnectAttempted = DateTime.Now;
            this.client = new TcpClient();
            client.ReceiveTimeout = 1000;
            this.client.Connect(address, port);
            NetworkStream ns = this.client.GetStream();
            this.netStream = new StreamWriter(ns);
        }

    }
}
