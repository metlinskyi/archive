using System;
using System.Text;

namespace Dr.Plugin.Pacients
{
    public class Pacient
    {
        public string Name { get; set; }

        StringBuilder _mBody;
        public String Body
        {
            get { return _mBody.ToString(); }
            set { _mBody = new StringBuilder(value); }
        }

        public Pacient()
        {
            _mBody = new StringBuilder();
        }

        public Pacient(String body)
        {
            _mBody = new StringBuilder(body);
        }

        public byte[] ToByte()
        {
            return ToByte(Encoding.Default);
        }

        public byte[] ToByte(Encoding encoding)
        {
            return encoding.GetBytes(Body);
        }

        public override string ToString()
        {
            return  Body;
        }

    }
}