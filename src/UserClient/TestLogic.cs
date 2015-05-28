using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Diagnostics;
using System.Windows.Forms;
using System.IO;
using System.Security.Cryptography;

namespace UserClient
{
    class TestLogic
    {

        public static void TestBandWidth(string sIP, int iMaxC)
        {

            WebClient wc = new WebClient();
            Stopwatch sw = new Stopwatch();
            sw.Start();
            for (int i = 1; i <= iMaxC; i++)
            {
                string sKey = i.ToString();
                string sUrl = "http://" + sIP + "/get/" + sKey;
                byte[] b = wc.DownloadData(sUrl);
            }
            sw.Stop();
            UIShowBox.LogBandWidth((sw.ElapsedTicks / (decimal)Stopwatch.Frequency).ToString());

            MessageBox.Show("Finish Test");

        }

        public static void TestMixBandWidth(int iNum1000m, int iNum100m, int iNum10m, string[] arIPs)
        {

            WebClient wc = new WebClient();
            Stopwatch sw = new Stopwatch();

            string sIP = arIPs[0];
            int iKey = 0;

            sw.Start();
            
            for (int i = 0; i < iNum1000m; i++)
            {
                iKey++;
                string sUrl = "http://" + sIP + "/get/" + iKey.ToString();
                byte[] b = wc.DownloadData(sUrl);
            }

            sIP = arIPs[1];
            for (int i = 0; i < iNum100m; i++)
            {
                iKey++;
                string sUrl = "http://" + sIP + "/get/" + iKey.ToString();
                byte[] b = wc.DownloadData(sUrl);
            }

            sIP = arIPs[2];
            for (int i = 0; i < iNum10m; i++)
            {
                iKey++;
                string sUrl = "http://" + sIP + "/get/" + iKey.ToString();
                byte[] b = wc.DownloadData(sUrl);
            }

            sw.Stop();

            UIShowBox.LogMixBandWidth((sw.ElapsedTicks / (decimal)Stopwatch.Frequency).ToString());

            //MessageBox.Show("Finish Test");

        }

        public static void EncryptDashFiles(int iBeg, int iEnd, string sInputPath, string sOutputPath, string sFileName, string sKey)
        {
            for (int i = iBeg; i <= iEnd; i++)
            {
                string sPath = sInputPath + @"\" + sFileName.Replace("{id}", i.ToString());
                byte[] FileIn = File.ReadAllBytes(sPath);
                UIShowBox.LogEncryptDash("Read " + sPath + "  :  " + FileIn.Length.ToString() + " ... ");
                byte[] FileOut = AES.EncryptAES(FileIn, sKey);
                UIShowBox.LogEncryptDash("Encrypt File " + FileOut.Length.ToString() + " Bytes..");
                string sOutPath = sOutputPath + @"\" + sFileName.Replace("{id}", i.ToString());
                UIShowBox.LogEncryptDash("Output the File : " + sOutPath);
                File.WriteAllBytes(sOutPath, FileOut);
            }
        }


        public static void TestDashLogic(string sIP, string sInput, string sEncKey, int iBeg, int iEnd, bool bEnc)
        {
            const int iMaxKind = 20;
            string[] arName = new string[iMaxKind];
            int[] iBand = new int[iMaxKind];

            string[] sAr = sInput.Split('\n');

            for (int i = 0; i < iMaxKind; i++)
            {
                string[] arStr = sAr[i].Split('\t');
                string sName = arStr[0];
                string sBand = arStr[1];

                arName[i] = arStr[0];
                iBand[i] = Int32.Parse(arStr[1]);
            }

            //have a test on the biggest one to work out the bandwidth
            string sKey = iBand[iMaxKind - 1].ToString() + "_1";

            WebClient wc = new WebClient();
            Stopwatch sw = new Stopwatch();
            sw.Start();
            byte[] b = wc.DownloadData(sIP + sKey);
            sw.Stop();

            decimal dtime = sw.ElapsedTicks / (decimal)Stopwatch.Frequency;
            int bandwidth = (int)((b.Length * 8) / dtime);

            UIShowBox.LogTestDash("The First Test Bandwidth is : " + bandwidth.ToString() + " bps");
            //main loop
            int k;
            MessageBox.Show("Begin to load the data!");
            for (int i = iBeg; i <= iEnd; i++)
            {
                //Check the next Package kind
                for (k = 0; k < iMaxKind; k++)
                {
                    if (iBand[k] > bandwidth)
                    {
                        break;
                    }
                }

                k = (k - 1) >= 0 ? k - 1 : 0;

                sKey = iBand[k].ToString() + "_" + i.ToString();
                sw = new Stopwatch();
                sw.Start();
                b = wc.DownloadData(sIP + sKey);
                if (bEnc)
                {
                    byte[] decByte = AES.DecryptAES(b, sEncKey);
                }
                sw.Stop();
                dtime = sw.ElapsedTicks / (decimal)Stopwatch.Frequency;
                bandwidth = (int)((b.Length * 8) / dtime);
                UIShowBox.LogTestDash("The [ " + i.ToString() + " ] Chunk Use time : " + dtime.ToString() + "  And Bandwidth [ " + bandwidth.ToString() + " ] .Turn to the [ " + k.ToString() + " ]");

                if (i == iEnd)
                {
                    i = iBeg;
                }

            }


        }

        


    }
}
