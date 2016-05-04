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

        public MouseStreamNet(bool enable = true, string address = "129.21.207.17", int port = 1313)
        {
            this.enabled = enable;
            if (this.enabled)
            {
                try
                {
                    this.client = new TcpClient(address, port);
                }
                catch (SocketException se)
                {
                    this.enabled = false;
                    Console.WriteLine("failed to connect to mouse");
                    Console.WriteLine(se.Message);
                    return;
                }
                NetworkStream ns = this.client.GetStream();
                this.netStream = new StreamWriter(ns);
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
            }
        }

    }
}
