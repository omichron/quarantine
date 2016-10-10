using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Grpc.Core;

using static Messenger.Messenger;
using System.Windows.Threading;

namespace GRPCMessenger
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        uint userId = 0;
        bool isConnected = false;
        MessengerClient client = null;
        Channel channel = null;
        ulong timeStampOffset = 0; // Needs to be added to the time
        ulong lastTimeStamp = 0;
        const int PORT = 6604;

        bool canCommunicate = true;

        DispatcherTimer refreshTimer = null;
        DispatcherTimer sendTimer = null;

        public MainWindow()
        {
            InitializeComponent();
        }

        ulong GetClientTimeStamp()
        {
            var epoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
            return Convert.ToUInt64((DateTime.Now - epoch).TotalMilliseconds);
        }

        ulong GetServerTimeStamp()
        {
            return GetClientTimeStamp() + timeStampOffset;
        }

        private async void buttonConnect_Click(object sender, RoutedEventArgs e)
        {
            if (isConnected) return;
            channel = new Channel(textHost.Text + ":" + PORT.ToString(), ChannelCredentials.Insecure);
            client = new MessengerClient(channel);

            var currentClientTimeStamp = GetClientTimeStamp();
            var loginRequest = new Messenger.LoginRequest { UserName = texUsername.Text };
            var reply = await client.LoginAsync(loginRequest);

            if (reply.Result.Result == Messenger.ResultReply.Types.Result.Failed) return;

            userId = reply.UserId;
            lastTimeStamp = reply.TimeStamp;
            timeStampOffset = lastTimeStamp - currentClientTimeStamp;

            refreshTimer = new System.Windows.Threading.DispatcherTimer();
            refreshTimer.Tick += new EventHandler(refreshTimer_Tick);
            refreshTimer.Interval = new TimeSpan(0, 0, 1);
            refreshTimer.Start();

            sendTimer = new System.Windows.Threading.DispatcherTimer();
            sendTimer.Tick += new EventHandler(sendTimer_Tick);
            sendTimer.Interval = new TimeSpan(0, 0, 0, 0, 100);
            sendTimer.Start();

            isConnected = true;
        }

        int sendCount = 0;
        private async void sendTimer_Tick(object sender, EventArgs e)
        {
            if (!isConnected) return;
            if (!canCommunicate) return;
            var textMessage = new Messenger.TextMessage
            {
                UserId = userId,
                TextMessage_ = (++sendCount).ToString(),
                TimeStamp = GetServerTimeStamp()
            };
            var result = await client.SendTextMessageAsync(textMessage);
        }

        private async void refreshTimer_Tick(object sender, EventArgs e)
        {
            if (!isConnected) return;
            if (!canCommunicate) return;
            canCommunicate = false;

            var pendingMessagesRequest = new Messenger.PendingMessagesRequest { TimeStamp = lastTimeStamp };
            var result = await client.RequestPendingMessagesAsync(pendingMessagesRequest);
            if (result.Result.Result == Messenger.ResultReply.Types.Result.Failed) return;

            foreach(var message in result.TextMessages)
            {
                textMessages.AppendText(message);
            }
            textMessages.ScrollToEnd();

            canCommunicate = true;
            lastTimeStamp = GetServerTimeStamp();
        }

        private async void buttonSend_Click(object sender, RoutedEventArgs e)
        {
            if (!isConnected) return;
            if (!canCommunicate) return;
            canCommunicate = false;

            var textMessage = new Messenger.TextMessage
            {
                UserId = userId,
                TextMessage_ = textMessageToSend.Text,
                TimeStamp = GetServerTimeStamp()
            };

            var result = await client.SendTextMessageAsync(textMessage);
            canCommunicate = true;
            if (result.Result == Messenger.ResultReply.Types.Result.Failed) return;
            textMessageToSend.Clear();
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (!isConnected) return;
            channel.ShutdownAsync().Wait();
        }
    }
}
