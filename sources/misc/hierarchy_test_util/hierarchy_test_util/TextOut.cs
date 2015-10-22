using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace hierarchy_test_util
{
    class TextOut
    {
        private string m_tabs = "";
        private StringBuilder m_strBuider = new StringBuilder();
        private bool m_newLine = true;

        public void Tab()
        {
            m_tabs += "\t";
        }

        public void Untab()
        {
            m_tabs = m_tabs.Substring(0, m_tabs.Length - 1);
        }

        public void NewLine()
        {
            m_strBuider.Append("\r\n");
            m_newLine = true;
        }

        public void AppendLine(string i_line)
        {
            if (m_newLine)
            {
                m_strBuider.Append(m_tabs);
            }
            m_strBuider.Append(i_line + "\r\n");
            m_newLine = true;
        }

        public void Append(string i_line)
        {
            if (m_newLine)
            {
                m_strBuider.Append(m_tabs);
                m_newLine = false;
            }
            m_strBuider.Append(i_line);
        }

        public override string ToString()
        {
            return m_strBuider.ToString();
        }
    }
}
