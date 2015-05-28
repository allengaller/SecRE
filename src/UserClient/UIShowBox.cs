using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace UserClient
{
    class UIShowBox
    {

        private static RichTextBox rtbBandWidth;
        private static RichTextBox rtbMixBandWidth;
        private static RichTextBox rtbEncryptDash;
        private static RichTextBox rtbTestDash;

        public static void Init(RichTextBox BandWidth, RichTextBox MixBandWidth, RichTextBox EncryptDash, RichTextBox TestDash)
        {
            
            rtbBandWidth = BandWidth;
            rtbMixBandWidth = MixBandWidth;
            rtbEncryptDash = EncryptDash;
            rtbTestDash = TestDash;

        }

        public static void LogBandWidth(string sMsg)
        {
            rtbBandWidth.Text += sMsg + "\n";
        }

        public static void LogMixBandWidth(string sMsg)
        {
            rtbMixBandWidth.Text += sMsg + "\n";
        }

        public static void LogEncryptDash(string sMsg)
        {
            rtbEncryptDash.Text += sMsg + "\n";
        }

        public static void LogTestDash(string sMsg)
        {
            rtbTestDash.Text += sMsg + "\n";
        }


    }
}
