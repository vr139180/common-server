using System;
using System.Collections.Generic;
using System.Text;

namespace com.cms.client.utils
{
    public class CommonUtil
    {
        public static string gen_uuid()
        {
            return Guid.NewGuid().ToString("N");
        }

        public static int cal_ctan(int sx, int sy, int ex, int ey, ref bool succ)
        {
            succ = true;

            int dty = Math.Abs(ey - sy);
            int dtx = Math.Abs(ex - sx);

            if (dty == 0 && dtx == 0)
            {
                succ = false;
                return 0;
            }

            double t = 0.0f;
            if (dtx != 0)
                t = Math.Atan(dty * 1.0f / dtx);
            t = t * 180 / Math.PI;

            if (ex > sx)
            {
                if (ey < sy)
                    t = 360 - t;
                else if (ey == sy)
                    t = 0;
            }
            else if (ex < sx)
            {
                if (ey >= sy)
                    t = 180 - t;
                else
                    t = 270 - t;
            }
            else
            {
                if (ey > sy)
                    t = 90;
                else
                    t = 270;
            }

            return (int)t;
        }

        public static int string2int(string val, int def = 0)
        {
            try
            {
                if (val == null)
                    return def;

                val =val.Trim();

                return int.Parse(val);
            }
            catch (Exception)
            {
                return def;
            }
        }

        public static void int2bytes(byte[] buf, int offset, int val)
        {
            buf[offset] = (byte)(val & 0x000000ff);
            buf[offset + 1] = (byte)((val & 0x0000ff00) >> 8);
            buf[offset + 2] = (byte)((val & 0x00ff0000) >> 16);
            buf[offset + 3] = (byte)((val & 0xff000000) >> 24);
        }

        public static void uint2bytes(byte[] buf, int offset, uint val)
        {
            buf[offset] = (byte)(val & 0x000000ff);
            buf[offset + 1] = (byte)((val & 0x0000ff00) >> 8);
            buf[offset + 2] = (byte)((val & 0x00ff0000) >> 16);
            buf[offset + 3] = (byte)((val & 0xff000000) >> 24);
        }

        public static void short2bytes(byte[] buf, int offset, short val)
        {
            buf[offset] = (byte)(val & 0x00ff);
            buf[offset + 1] = (byte)((val & 0xff00) >> 8);
        }

        public static void ushort2bytes(byte[] buf, int offset, ushort val)
        {
            buf[offset] = (byte)(val & 0x00ff);
            buf[offset + 1] = (byte)((val & 0xff00) >> 8);
        }

        public static void long2bytes(byte[] buf, int offset, long val)
        {
            byte[] b = BitConverter.GetBytes(val);

            Buffer.BlockCopy(b, 0, buf, offset, b.Length);
        }

        public static void ulong2bytes(byte[] buf, int offset, ulong val)
        {
            byte[] b = BitConverter.GetBytes(val);

            Buffer.BlockCopy(b, 0, buf, offset, b.Length);
        }

        public static void float2bytes(byte[] buf, int offset, double val)
        {
            byte[] b =BitConverter.GetBytes(val);
            Buffer.BlockCopy(b, 0, buf, offset, b.Length);
        }

        public static bool buff2short(byte[] buff, int startpos, ref short val)
        {
            try
            {
                val = BitConverter.ToInt16(buff, startpos);
            }
            catch (Exception) { return false; }

            return true;
        }

        public static bool buff2ushort(byte[] buff, int startpos, ref ushort val)
        {
            try
            {
                val = BitConverter.ToUInt16(buff, startpos);
            }
            catch (Exception) { return false; }

            return true;
        }
    }
}
