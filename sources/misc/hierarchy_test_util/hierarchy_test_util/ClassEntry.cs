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
        private bool m_hasVTable, m_hasMDTFunc;
        private IEnumerable<ClassEntry> m_bases;
        private List<ClassEntry> m_nonVirtualBases = new List<ClassEntry>();
        private List<ClassEntry> m_virtualBases = new List<ClassEntry>();
        private List<ClassProperty> m_properties;
        
        public ClassEntry(string i_parentNamespace, string i_name, bool i_hasVTable, bool i_hasMDTFunc,
            List<ClassProperty> i_props)
        {
            m_bases = m_nonVirtualBases.Concat(m_virtualBases);
            m_hasVTable = i_hasVTable;
            m_hasMDTFunc = i_hasMDTFunc;
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

        public IEnumerable<ClassEntry> Bases
        {
            get { return m_bases; }
        }

        public void AddBase(ClassEntry i_base, bool i_virtual)
        {
            if(!i_virtual)
            {
                m_nonVirtualBases.Add(i_base);
            }
            else
            {
                m_virtualBases.Add(i_base);
            }                
        }

        public void RemoveBase(ClassEntry i_base, bool i_virtual)
        {
            if (!i_virtual)
            {
                m_nonVirtualBases.Remove(i_base);
            }
            else
            {
                m_virtualBases.Remove(i_base);
            }
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

        public ClassEntry GetRandomBaseOrThis(Random i_rand)
        {
            List<ClassEntry> bases = new List<ClassEntry>();
            List<ClassEntry> virtualBases = new List<ClassEntry>();
            GetAllBases(bases, virtualBases);
            virtualBases.RemoveDupicates();
            bases.AddRange(virtualBases);
            bases.Add(this);
            return bases[i_rand.Next(bases.Count)];
        }
        
        public void GetAllBases(List<ClassEntry> i_nonVirtualBases, List<ClassEntry> i_virtualBases)
        {
            i_nonVirtualBases.AddRange(m_nonVirtualBases);
            i_virtualBases.AddRange(m_virtualBases);

            foreach ( ClassEntry baseC in m_bases )
            {
                baseC.GetAllBases(i_nonVirtualBases, i_virtualBases);
            }            
        }

        public void WriteDefinition(TextOut i_output, bool i_emitTypeChecks)
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
         
            i_output.AppendLine("public:");
            
            // vtable
            if (m_hasVTable)
            {                
                i_output.AppendLine("virtual ~" + Name + "() {}");
                i_output.AppendLine("virtual int vfunct_" + Name + "() { return 42; }");
            }

            if (i_emitTypeChecks)
            {
                i_output.AppendLine("std::string type_name = \"" + Name + "\";");
            }

            if(m_hasMDTFunc)
            {
                i_output.AppendLine("virtual const reflective::Type & virtual_get_type() const");
                i_output.AppendLine("\t{ return reflective::get_naked_type<" +Name + ">(); }");
                i_output.AppendLine("");
                i_output.AppendLine("static const reflective::Type & static_get_type(const void * i_object)");
                i_output.AppendLine("\t{ return static_cast<const " + Name + " *>(i_object)->virtual_get_type(); }");
            }

            // properties
            foreach (ClassProperty prop in m_properties)
            {
                i_output.AppendLine(prop.Type + " " + prop.Name + ";" );
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

            // bases
            ClassEntry[] bases = m_bases.ToArray();
            int baseCount = bases.Length;
            if (baseCount > 0)
            {
                i_output.AppendLine("i_context.type()->set_base_types( {");
                i_output.Tab();

                for (int baseIndex = 0; baseIndex < baseCount; baseIndex++)
                {
                    ClassEntry baseClass = bases[baseIndex];
                    string term;
                    if (baseIndex + 1 < baseCount)
                        term = ",";
                    else
                        term = " });";
                    i_output.AppendLine("BaseType::from_types<ThisClass, " + baseClass.FullName + ">()" + term);
                }
                i_output.Untab();
            }

            // gmdt function
            i_output.AppendLine("i_context.type()->set_most_derived_type_func(&ThisClass::static_get_type);");

            i_output.Untab();
            i_output.AppendLine("}");
        }
    }
}
