using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography;

namespace UserClient
{
    class AES
    {
        #region EncryptOperate

        public static byte[] DecryptAES(byte[] input, string key)
        {
            SHA256 sha256 = new SHA256CryptoServiceProvider();
            //Make a Key
            byte[] source = Encoding.UTF8.GetBytes(key);
            byte[] encKey = sha256.ComputeHash(source);
            //Make a IV
            source = Encoding.UTF8.GetBytes(key + "IV");
            byte[] encIV = sha256.ComputeHash(source);
            byte[] encRightIV = new byte[16];
            for (int i = 0; i < encRightIV.Length; i++)
            {
                encRightIV[i] = encIV[i];
            }

            RijndaelManaged aes = new RijndaelManaged();
            aes.Key = encKey;
            aes.IV = encRightIV;
            aes.Mode = CipherMode.CBC;
            aes.Padding = PaddingMode.PKCS7;

            ICryptoTransform transform = aes.CreateDecryptor();
            return transform.TransformFinalBlock(input, 0, input.Length);

        }

        public static byte[] EncryptAES(byte[] input, string key)
        {
            SHA256 sha256 = new SHA256CryptoServiceProvider();
            //Make a Key
            byte[] source = Encoding.UTF8.GetBytes(key);
            byte[] encKey = sha256.ComputeHash(source);
            //Make a IV
            source = Encoding.UTF8.GetBytes(key + "IV");
            byte[] encIV = sha256.ComputeHash(source);
            byte[] encRightIV = new byte[16];
            for (int i = 0; i < encRightIV.Length; i++)
            {
                encRightIV[i] = encIV[i];
            }

            RijndaelManaged aes = new RijndaelManaged();
            aes.Key = encKey;
            aes.IV = encRightIV;
            aes.Mode = CipherMode.CBC;
            aes.Padding = PaddingMode.PKCS7;

            ICryptoTransform transform = aes.CreateEncryptor();
            return transform.TransformFinalBlock(input, 0, input.Length);
        }

        #endregion
    }
}
