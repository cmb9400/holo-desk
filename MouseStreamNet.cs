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

        public MouseStreamNet()
        {
            this.client = new TcpClient("8.8.8.8", 53);
            NetworkStream ns = this.client.GetStream();
            this.netStream = new StreamWriter(ns);
        }

        public void DataReceived (object sender, DataReceivedEventArgs args)
        {
            Console.WriteLine(args.Data);
            try
            {
                this.netStream.Write(args.Data);
                this.netStream.Flush();
            } catch (IOException e)
            {
                Console.Error.WriteLine(e.Message);
            }
        }

    }
}
