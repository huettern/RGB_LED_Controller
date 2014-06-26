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
using System.IO.Ports;


namespace Solution_CS_WPF
{

    
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        #region variables
        int cnt = 0;
        //Serial
        SerialPort SerialPort = new SerialPort();

        const byte TELE_SIZE = 10;

        #endregion

        public MainWindow()
        {
            InitializeComponent();
            //create timer function
            System.Windows.Threading.DispatcherTimer dispatcherTimer = new System.Windows.Threading.DispatcherTimer();
            dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
            dispatcherTimer.Interval = TimeSpan.FromSeconds(0.2);
            dispatcherTimer.Start();


            //Serial Com
            string[] SerialPortsAuto = System.IO.Ports.SerialPort.GetPortNames();
            
            string[] SerialPorts;
            SerialPorts = new string[10];
            SerialPorts[0] = "COM1";
            SerialPorts[1] = "COM4";
            SerialPorts[2] = "COM9";
            foreach (string name in SerialPortsAuto)
            {
                ComList.Items.Add(name);
            }
            ChannelBox.Items.Add("Kanal 1");
        }
        
        private void BtnConnect_Click(object sender, RoutedEventArgs e)
        {
            if((string)BtnConnect.Content == "Connect")
            {
                try
                {
                    //Sets up serial port
                    SerialPort.PortName = ComList.Text;
                    SerialPort.BaudRate = Convert.ToInt32(38400);
                    SerialPort.Handshake = System.IO.Ports.Handshake.None;
                    SerialPort.Parity = Parity.None;
                    SerialPort.DataBits = 8;
                    SerialPort.StopBits = StopBits.One;
                    SerialPort.ReadTimeout = 200;
                    SerialPort.WriteTimeout = 50;
                    SerialPort.Open();
                    BtnConnect.Content = "Disconnect";
                }
                catch (Exception ex)
                {
                    SerialOut.Text += "\n---------------"+"\nError Opening Port!\n";
                    SerialOut.Text += ex.Message + "\n---------------\n\n";
                }
            }
            else
            {
                SerialPort.Close();
                BtnConnect.Content = "Connect";
            }
        }

        private void Slider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            fUpdateColor();
        }

        /// <summary>
        /// Updates Color Monitor Fill
        /// </summary>
        private void fUpdateColor ()
        {
            byte R, G, B;
            R = Convert.ToByte(RSlider.Value / 257);
            G = Convert.ToByte(GSlider.Value / 257);
            B = Convert.ToByte(BSlider.Value / 257);
            ColorMonitor.Fill = new SolidColorBrush( Color.FromRgb(R, G, B) );
        }

        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            byte[] SendBytes;
            ushort value;
            SendBytes = new byte[TELE_SIZE];
            SendBytes[0] = 0xCA;    //start byte
            SendBytes[1] = 0;    //cmd byte
            SendBytes[2] = 0;    //channel byte


            value = (ushort)RSlider.Value;
            SendBytes[3] = Convert.ToByte((value >> 8) & 0x00FF);    //data byte
            SendBytes[4] = Convert.ToByte(value & 0x00FF);    //data byte

            value = (ushort)GSlider.Value;
            SendBytes[5] = Convert.ToByte((value >> 8) & 0x00FF);    //data byte
            SendBytes[6] = Convert.ToByte(value & 0x00FF);    //data byte

            value = (ushort)BSlider.Value;
            SendBytes[7] = Convert.ToByte((value >> 8) & 0x00FF);    //data byte
            SendBytes[8] = Convert.ToByte(value & 0x00FF);    //data byte

            SendBytes[9] = 42;

            ColorOut.Fill = new SolidColorBrush(Color.FromRgb(
                SendBytes[3],
                SendBytes[5],
                SendBytes[7]));

            // send
            foreach (byte data in SendBytes)
            {
                SerialOut.Text += Convert.ToString(data) + " ";
            }
            SerialOut.Text += "\n";
            SerialOut.ScrollToEnd();
            if(SerialPort.IsOpen == true)
                SerialPort.Write(SendBytes, 0, TELE_SIZE);
        }

        private void ComList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            BtnConnect.IsEnabled = true;
        }

        private void ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }
    }
}
