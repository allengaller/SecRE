using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Diagnostics;
using System.Threading;
using System.Security.Cryptography;
using System.IO;

namespace UserClient
{
    public partial class UserClientTest : Form
    {
        public UserClientTest()
        {
            InitializeComponent();
            //Just for Demo.
            Control.CheckForIllegalCrossThreadCalls = false;
        }

        string sIP;
        string[] arIPs;

        #region BandWidthZone

        private void rbBandWidth_1000M_CheckedChanged(object sender, EventArgs e)
        {
            //The first
            sIP = arIPs[0];
        }

        private void rbBandWidth_100M_CheckedChanged(object sender, EventArgs e)
        {
            //The Second
            sIP = arIPs[1];
        }

        private void rbBandWidth_10M_CheckedChanged(object sender, EventArgs e)
        {
            //The Third
            sIP = arIPs[2];
        }

        private void btnBandWidth_Click(object sender, EventArgs e)
        {
            int iMaxCount = Int32.Parse(tbBandWidth_MaxCount.Text);

            TestLogic.TestBandWidth(sIP, iMaxCount);

            //new Thread(() => TestLogic.TestBandWidth(sIP, iMaxCount)).Start();
        }

        #endregion

        #region MixBandWidthZone


        private void btnMixBandWidth_Click(object sender, EventArgs e)
        {

            int iNum1000m = (int)nudMixBandWidth_1000M.Value;
            int iNum100m = (int)nudMixBandWidth_100M.Value;
            int iNum10m = (int)nudMixBandWidth_10M.Value;

            TestLogic.TestMixBandWidth(iNum1000m, iNum100m, iNum10m, arIPs);

            //new Thread(() => TestLogic.TestMixBandWidth(iNum1000m, iNum100m, iNum10m, arIPs)).Start();

        }


        #endregion

        #region EncryptDash

        private void btnEncryptDashIn_Click(object sender, EventArgs e)
        {
            DialogResult dr = folderBrowserDialog1.ShowDialog();
            if (DialogResult.OK != dr)
            {
                return;
            }
            tbEncrypDashInput.Text = folderBrowserDialog1.SelectedPath;

        }

        private void btnEncryptDashOut_Click(object sender, EventArgs e)
        {
            DialogResult dr = folderBrowserDialog1.ShowDialog();
            if (DialogResult.OK != dr)
            {
                return;
            }
            tbEncryptDashOutput.Text = folderBrowserDialog1.SelectedPath;
        }

        private void btnEncryptDash_Click(object sender, EventArgs e)
        {
            //make a key from sha256

            int iBeg = Int32.Parse(tbEncrypDashBeg.Text);
            int iEnd = Int32.Parse(tbEncryptDashEnd.Text);
            string sInputPath = tbEncrypDashInput.Text;
            string sOutputPath = tbEncryptDashOutput.Text;
            string sFileName = tbEncryptDashFileName.Text;
            string sKey = tbEncryptDashKey.Text;

            TestLogic.EncryptDashFiles(iBeg, iEnd, sInputPath, sOutputPath, sFileName, sKey);

        }

        #endregion

        #region TestDash

        private void btnTestDash_Click(object sender, EventArgs e)
        {
            int iBeg = Int32.Parse(tbTestDashBeg.Text);
            int iEnd = Int32.Parse(tbTestDashEnd.Text);
            bool bEnc = cbTestDashEnc.Checked;

            //Run the test and you should also run the Limit.sh to limit the bandwidth.
            string sIP = arIPs[0];
            string sInput = rtbTestDashInput.Text;
            string sEncKey = tbEncryptDashKey.Text;

            TestLogic.TestDashLogic(sIP, sInput, sEncKey, iBeg, iEnd, bEnc);


        }

        private void UserClientTest_Load(object sender, EventArgs e)
        {
            //Set the UI Control
            UIShowBox.Init(rtbBandWidthBox, rtbMixBandWidthBox, rtbEncrypDashBox, rtbTestDashOutput);

            //Set the IP.
            arIPs = new string[] { "192.168.0.30", "192.168.0.20", "192.168.0.10" };
        }


        #endregion

    }
}
