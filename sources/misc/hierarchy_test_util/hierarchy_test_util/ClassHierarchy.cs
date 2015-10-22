using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace hierarchy_test_util
{
    class ClassHierarchy
    {
        private string m_hierarchyName;
        private List<ClassEntry> m_classes = new List<ClassEntry>();

        public ClassHierarchy(string i_hierarchyName, int i_classCount)
        {
            m_hierarchyName = i_hierarchyName;
            for( int classIndex = 1; classIndex <= i_classCount; classIndex++)
            {
                m_classes.Add(new ClassEntry("reflective::details::" + i_hierarchyName, "Class_" + classIndex.ToString()));
            }
        }

        public void AddRandomBaseClasses(int i_derivationCount)
        {
            Random rand = new Random();
            int currDer = 0;
            while(currDer < i_derivationCount)
            {
                int firstClass = rand.Next(m_classes.Count - 1);
                int secondClass = rand.Next(firstClass + 1, m_classes.Count);
                if( !m_classes[secondClass].HasBaseClass(m_classes[firstClass]) )
                {
                    m_classes[secondClass].Bases.Add(m_classes[firstClass]);
                    currDer++;
                }
            }            
        }

        public string GetCode()
        {
            TextOut output = new TextOut();
            
            output.AppendLine("namespace reflective");
            output.AppendLine("{");
            output.Tab();

            output.AppendLine("namespace details");
            output.AppendLine("{");
            output.Tab();

            output.AppendLine("namespace " + m_hierarchyName);
            output.AppendLine("{");
            output.Tab();

            foreach ( ClassEntry classEntry in m_classes)
            {
                classEntry.WriteDefinition(output);
            }

            output.Untab();
            output.AppendLine("};");

            output.Untab();
            output.AppendLine("};");

            output.AppendLine("");
            foreach (ClassEntry classEntry in m_classes)
            {
                classEntry.WriteReflection(output);
            }
            output.Untab();
            output.AppendLine("};");


            return output.ToString();
        }
    }
}
