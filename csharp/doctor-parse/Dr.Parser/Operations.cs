using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;

namespace Dr.Parser
{
    public class Operations : Settings
    {
        readonly List<YamlNode> _mOperations = new List<YamlNode>();

        public Operations(string source)
        {
            TextReader reader = null;
            try
            {
                reader = new StringReader(source);
                Parse(ref reader);
            }
            catch (Exception ex)
            {
                var e = ex;
            }
            finally
            {
                if (reader != null)
                    reader.Close();
            }
        }

        public Operations(TextReader source)
        {
            try
            {
                Parse(ref source);
            }
            catch (Exception ex)
            {
                var e = ex;
            }
            finally
            {
                if (source != null)
                    source.Close();
            }
        }

        public Operations(FileInfo source)
        {
            if (!source.Exists)
            {
                return;
            }

            TextReader reader = null;
            try
            {
                reader = new StreamReader(source.FullName);
                Parse(ref reader);
            }
            catch (Exception ex)
            {
                var e = ex;
            }
            finally
            {
                if (reader != null)
                    reader.Close();
            }
        }

        public List<YamlNode> List
        {
            get { return _mOperations; }
        }

        private void Parse(ref TextReader reader)
        {
            YamlNode node = null;

            String sequence = string.Empty;
            String name = string.Empty;

            bool isParametr = false;

            var line = new StringBuilder();
            while (reader.Peek() >= 0)
            {
                line.Remove(0, line.Length);
                line.AppendLine(reader.ReadLine());

                var match = Regex.Match(line.ToString(), @"^(.*?):", RegexOptions.Compiled);
                if (match.Success)
                {
                    name = match.Value.Replace(":", string.Empty).Trim();
                    if ((match.Value.Substring(0, 1) != " ") && (match.Value.Substring(0, 1) != "-"))
                    {
                        sequence = name;
                        continue;
                    }
                }

                var value = line.Remove(match.Index, match.Length).ToString();
                if (match.Success)
                {
                    if (name.Contains("-"))
                    {
                        isParametr = true;

                        if (node != null) 
                            node.Parameters.Add(name.Replace("-", string.Empty).Trim(), value.Trim());
                    }
                    else
                    {
                        isParametr = false;

                        node = new YamlNode(name) {Value = value.Trim()};

                        switch (sequence)
                        {
                            case "Settings":
                                m_Settings.Add(node.Name, node);
                                break;
                            case "Operations":
                                _mOperations.Add(node);
                                break;
                        }
                    }
                }
                else
                {
                    if (node != null)
                        if (isParametr)
                            node.Parameters[name.Replace("-", string.Empty).Trim()] += value;
                        else
                            node.Value += value;
                }
                line.Remove(0, line.Length);
            } //while (reader.Peek() >= 0)
        }

    }
}