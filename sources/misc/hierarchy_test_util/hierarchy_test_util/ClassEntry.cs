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
        private bool m_hasVTable;
        private List<ClassEntry> m_bases = new List<ClassEntry>();
        private List<ClassProperty> m_properties;
        private HashSet<ClassEntry> m_virtualBases = new HashSet<ClassEntry>();

        public ClassEntry(string i_parentNamespace, string i_name, bool i_hasVTable,
            List<ClassProperty> i_props)
        {
            m_hasVTable = i_hasVTable;
            m_parentNamespace = i_parentNamespace;
            m_name = i_name;
            m_properties = i_props;
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

        public void AddBase(ClassEntry i_base, bool i_virtual)
        {
            m_bases.Add(i_base);
            if( i_virtual )
                m_virtualBases.Add(i_base);
        }

        public bool HasBaseClass(ClassEntry i_other)
        {
            foreach(ClassEntry baseClass in m_bases )
            {
                if (baseClass == i_other)
                    return true;

                if (baseClass.HasBaseClass(i_other))
                    return true;
            }
            return false;
        }

        public HashSet<ClassEntry> GetAllBases()
        {
            HashSet<ClassEntry> set = new HashSet<ClassEntry>();
            GetAllBases(set);
            return set;
        }

        private void GetAllBases(HashSet<ClassEntry> i_dest)
        {
            foreach( ClassEntry baseC in m_bases )
            {
                i_dest.Add(baseC);
                baseC.GetAllBases(i_dest);
            }            
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
                if( m_virtualBases.Contains(baseEntry) )
                {
                    i_output.Append("virtual ");
                }
                i_output.Append(baseEntry.Name);
                first = false;
            }
            i_output.NewLine();
            i_output.AppendLine("{");
            i_output.Tab();
            if( m_hasVTable )
            {
                i_output.AppendLine("public:");
                i_output.AppendLine("virtual ~" + Name + "() {}");
                i_output.AppendLine("virtual int vfunct_" + Name + "() { return 42; }");

                foreach(ClassProperty prop in m_properties)
                {
                    i_output.AppendLine(prop.Type + " " + prop.Name + ";" );
                }
            }
            i_output.Untab();
            i_output.AppendLine("};");
        }

        public void WriteReflection(TextOut i_output)
        {
            i_output.Append("void setup_type(TypeSetupContext<" + FullName + "> & i_context)");
            i_output.NewLine();
            i_output.AppendLine("{");
            i_output.Tab();
            i_output.AppendLine("using ThisClass = " + FullName + ";");



            if (m_bases.Count > 0)
            {
                i_output.AppendLine("i_context.type()->set_base_types( {");
                i_output.Tab();

                for (int baseIndex = 0; baseIndex < m_bases.Count; baseIndex++)
                {
                    ClassEntry baseClass = m_bases[baseIndex];
                    string term;
                    if (baseIndex + 1 < m_bases.Count)
                        term = ",";
                    else
                        term = " });";
                    i_output.AppendLine("BaseType::from_types<ThisClass, " + baseClass.FullName + ">()" + term);
                }
                i_output.Untab();
            }

            i_output.Untab();
            i_output.AppendLine("};");
        }
    }
}
