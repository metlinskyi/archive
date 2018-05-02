using System.IO;
using System.Text;

namespace HtmlNormalizer
{
    public class HtmlStream : Stream
    {
        private HtmlNormalizer _normalizer;

        private Stream _stream;

        private MemoryStream _memory;

        private long _position;

        public HtmlStream(Stream stream)
        {
            _stream = stream;
            _memory = new MemoryStream(8192);
            _normalizer = new HtmlNormalizer();
        }

        #region Stream overrides

        public override bool CanRead
        {
            get { return true; }
        }

        public override bool CanSeek
        {
            get { return true; }
        }

        public override bool CanWrite
        {
            get { return true; }
        }

        public override void Close()
        {
            _stream.Close();
        }

        public override void Flush()
        {
            if (_memory.Length > 0)
            {
                string content = Encoding.UTF8.GetString(_memory.GetBuffer(), 0, (int)_memory.Length);

                byte[] data = Encoding.UTF8.GetBytes(_normalizer.Process(content));

                _stream.Write(data, 0, data.Length);

                _memory = new MemoryStream();
            }

            _stream.Flush();
        }

        public override long Length
        {
            get { return 0; }
        }

        public override long Position
        {
            get { return _position; }
            set { _position = value; }
        }

        public override long Seek(long offset, SeekOrigin origin)
        {
            return _stream.Seek(offset, origin);
        }

        public override void SetLength(long length)
        {
            _stream.SetLength(length);
        }

        public override int Read(byte[] buffer, int offset, int count)
        {
            return _stream.Read(buffer, offset, count);
        }

        public override void Write(byte[] buffer, int offset, int count)
        {
            _stream.Write(buffer, offset, count);
        }

        #endregion
    }
}
