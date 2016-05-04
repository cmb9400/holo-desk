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

        public MouseStreamNet(bool enable = true)
        {
            this.enabled = enable;
            if (enable)
            {
                this.client = new TcpClient("129.21.207.17", 1313);
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
            } catch (IOException e)
            {
                Console.Error.WriteLine(e.Message);
            }
        }

    }
}
