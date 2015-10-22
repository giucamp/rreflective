using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace hierarchy_test_util
{
    class ClassEntry
    {
        private string m_parentNamespace;
        private string m_name;
        private List<ClassEntry> m_bases = new List<ClassEntry>();

        public ClassEntry(string i_parentNamespace, string i_name)
        {
            m_parentNamespace = i_parentNamespace;
            m_name = i_name;
        }

        public string Name
        {
            get { return m_name; }
        }

        public string FullName
        {
            get { return m_parentNamespace.Length > 0 ? m_parentNamespace +"::" + m_name : m_name; }
        }

        public List<ClassEntry> Bases
        {
            get { return m_bases; }
        }

        public bool HasBaseClass(ClassEntry i_other)
        {
            foreach(ClassEntry baseClass in m_bases )
            {
                if (baseClass == i_other)
                    return true;

                if (baseClass.HasBaseClass( i_other))
                    return true;
            }
            return false;
        }

        public void WriteDefinition(TextOut i_output)
        {
            i_output.Append("class " + Name);

            bool first = true;
            foreach( ClassEntry baseEntry in m_bases )
            {
                if( first )
                {
                    i_output.Append( " : public " );
                }
                else
                {
                    i_output.Append(", public ");
                }
                i_output.Append(baseEntry.Name);
                first = false;
            }
            i_output.NewLine();
            i_output.AppendLine("{");
            i_output.Tab();
            i_output.Untab();
            i_output.AppendLine("};");
        }

        public void WriteReflection(TextOut i_output)
        {
            i_output.Append("void setup_type(TypeSetupContext<" + FullName + "> & i_context)");
            i_output.NewLine();
            i_output.AppendLine("{");
            i_output.Tab();
            i_output.Untab();
            i_output.AppendLine("};");
        }
    }
}
