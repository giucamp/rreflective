using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace hierarchy_test_util
{
    public class HierarchySettings
    {
        public string HierarchyName { get; set; }
        public double VTableChance { get; set; }
        public double VBaseChance { get; set; }
        public double MostDerivedTypeFuncChance { get; set; }
        public int ClassCount { get; set; }
        public int DerivationFactor { get; set; }
        public bool AllowMultipleInheritance { get; set; }
        public int TestCount { get; set; }
        public bool EmitTypeChecks { get; set; }

        public HierarchySettings(string i_hierarchyName)
        {
            HierarchyName = i_hierarchyName;
            AllowMultipleInheritance = true;
            ClassCount = 10;
            DerivationFactor = 5;
            VTableChance = 0.5;
            MostDerivedTypeFuncChance = 1.5;
            VBaseChance = 0;
            TestCount = 1000;
            EmitTypeChecks = true;
        }
    }

    public class ClassHierarchy
    {
        private HierarchySettings m_settings;
        private Random m_rand = new Random(3);
        private List<ClassEntry> m_classes = new List<ClassEntry>();

        private PropertyFactory m_propFactory;
        
        public ClassHierarchy(HierarchySettings i_settings)
        {
            m_settings = i_settings;
            m_propFactory = new PropertyFactory(m_rand);
            for ( int classIndex = 0; classIndex < i_settings.ClassCount; classIndex++)
            {
                List<ClassProperty> props = new List<ClassProperty>();
                int propCount = m_rand.Next(10);
                for(int propIndex = 0; propIndex < propCount; propIndex++)
                {
                    props.Add(m_propFactory.NewProp(.7, 0.0, 28));
                }

                bool hasVTable = m_rand.NextDouble() < m_settings.VTableChance;
                bool hasMDTFunc = m_rand.NextDouble() < m_settings.MostDerivedTypeFuncChance;
                m_classes.Add(new ClassEntry("reflective::details::" + i_settings.HierarchyName, "Class_" + classIndex.ToString(), hasVTable, hasMDTFunc, props));
            }

            AddRandomBaseClasses();
        }

        public string FileName { get; set; }

        public HierarchySettings Settings
        {
            get
            {
                return m_settings;
            }
        }

        public void Save()
        {
            System.IO.File.WriteAllText(FileName, GenerateSourceCode() );
        }

        bool HasAmbiguity()
        {
            foreach(ClassEntry currClass in m_classes)
            {
                List<ClassEntry> nonVirtualBases = new List<ClassEntry>();
                List<ClassEntry> virtualBases = new List<ClassEntry>();
                currClass.GetAllBases(nonVirtualBases, virtualBases);
                virtualBases.RemoveDupicates();

                if( nonVirtualBases.Concat(virtualBases).HasDupicates() )
                {
                    return true;
                }
            }
            return false;
        }

        private void AddRandomBaseClasses()
        {
            int fails = 0;
            while(fails < m_classes.Count * m_settings.DerivationFactor)
            {
                int i = m_rand.Next(m_classes.Count);
                int j = m_rand.Next(m_classes.Count);
                if( i > j )
                {
                    Utils.Swap(ref i, ref j);
                }
                else if(i==j)
                {
                    continue;
                }

                bool success = false;

                ClassEntry baseC = m_classes[i];
                ClassEntry derC = m_classes[j];
                if (!derC.Bases.Contains(baseC))
                {
                    bool isVirtualBase = m_rand.NextDouble() < m_settings.VBaseChance;
                    derC.AddBase(baseC, isVirtualBase);
                    if (!HasAmbiguity())
                    {
                        success = true;                        
                    }
                    else
                    {
                        derC.RemoveBase(baseC, isVirtualBase);
                    }
                }

                if (success)
                {
                    fails = 0;
                }
                else
                {
                    fails++;
                }
            } 
        }

        ClassEntry RandClass()
        {
            return m_classes[m_rand.Next(m_classes.Count)];
        }

        private void GenerateTest(TextOut i_output)
        {
            i_output.AppendLine("namespace reflective");
            i_output.AppendLine("{");
            i_output.Tab();

            if (m_settings.EmitTypeChecks)
            {
                foreach (ClassEntry currClass in m_classes)
                {
                    i_output.AppendLine("template <> inline void dbg_object_validate(const " + currClass.FullName + " & i_object)");
                    i_output.AppendLine("{");
                    i_output.Tab();
                    i_output.AppendLine("REFLECTIVE_INTERNAL_ASSERT(i_object.type_name == \"" + currClass.Name + "\");");
                    i_output.Untab();
                    i_output.AppendLine("}");
                }
            }

            i_output.AppendLine("namespace details");
            i_output.AppendLine("{");
            i_output.Tab();

            i_output.AppendLine("namespace " + m_settings.HierarchyName);
            i_output.AppendLine("{");
            i_output.Tab();

            i_output.AppendLine("void test_" + m_settings.HierarchyName + "_test()");
            i_output.AppendLine("{");
            i_output.Tab();
            
            i_output.AppendLine("// objects");
            foreach (ClassEntry classObj in m_classes)
            {
                i_output.AppendLine(classObj.Name + " obj_" + classObj.Name + ";");
            }
            i_output.AppendLine("");


            i_output.AppendLine("");
            i_output.AppendLine("// test upcast and dynamic cast");
            i_output.AppendLine("");
            i_output.AppendLine("");
            for (int i = 0; i < m_settings.TestCount; i++)
            {
                ClassEntry completeClassObj = RandClass();
                ClassEntry baseClass = completeClassObj.GetRandomBaseOrThis(m_rand);
                ClassEntry destClass = RandClass();
                i_output.AppendLine("// upcasting from " + completeClassObj.Name + " to " + baseClass.Name + ", and the dynamic casting to " + destClass.Name);

                // complete object
                i_output.AppendLine(completeClassObj.Name + " * native_complete_ptr_" + i.ToString() + " = &obj_" + completeClassObj.Name + ";");
                i_output.AppendLine("ObjPtr ref_complete_ptr_" + i.ToString() + " = &obj_" + completeClassObj.Name + ";");

                // upcast
                i_output.AppendLine(baseClass.Name + " * native_base_ptr_" + i.ToString() + " = native_complete_ptr_" + i.ToString() + ";");
                i_output.AppendLine("ObjPtr ref_base_ptr_" + i.ToString() + " = ref_complete_ptr_" + i.ToString() + ".upcast_to(get_type<" + baseClass.Name + ">());");
                i_output.AppendLine("REFLECTIVE_INTERNAL_ASSERT(" +
                    "native_base_ptr_" + i.ToString() + " == " +
                    "static_cast<" + baseClass.Name + "*>( ref_base_ptr_" + i.ToString() + ".object() )" +
                    ");");
                i_output.AppendLine("reflective::dbg_object_validate(*ref_base_ptr_" + i.ToString() + ".get_if_type_matches<" + baseClass.Name + ">());");
                

                // dynamic_cast
                i_output.AppendLine(destClass.Name + " * native_dyn_ptr_" + i.ToString() + " = " + "dynamic_cast<" + destClass.Name + "*>(native_base_ptr_" + i.ToString() + ");");
                i_output.AppendLine("ObjPtr ref_dyn_ptr_" + i.ToString() + " = ref_base_ptr_" + i.ToString() + ".cast_to(get_type<" + destClass.Name + ">());");                
                i_output.AppendLine("REFLECTIVE_INTERNAL_ASSERT(" +
                    "native_dyn_ptr_" + i.ToString() + " == " +
                    "static_cast<" + destClass.Name + "*>( ref_dyn_ptr_" + i.ToString() + ".object() )" +
                    ");");
                i_output.AppendLine("if( !ref_dyn_ptr_" + i.ToString() + ".empty() )");
                i_output.AppendLine("{");
                i_output.Tab();
                i_output.AppendLine("reflective::dbg_object_validate(*ref_dyn_ptr_" + i.ToString() + ".get_if_type_matches<" + destClass.Name + ">());");                
                i_output.Untab();
                i_output.AppendLine("}");
                i_output.AppendLine("");
            }
            i_output.AppendLine("");
            
            i_output.Untab();
            i_output.AppendLine("}");

            i_output.Untab();
            i_output.AppendLine("}");

            i_output.Untab();
            i_output.AppendLine("}");

            i_output.Untab();
            i_output.AppendLine("}");
        }

        public string GenerateSourceCode()
        {
            TextOut output = new TextOut();
            output.AppendLine("");
            output.AppendLine("// This source file was generated by hierarchy_test_util");
            output.AppendLine("");
            output.AppendLine("// incudes");
            output.AppendLine(@"#include <string>");
            output.AppendLine(@"#include <iostream>");
            output.AppendLine(@"#include <cstdlib>");
            output.AppendLine(@"#include ""..\sources\reflective.h""");
            output.AppendLine(@"");
            output.AppendLine(@"");

            output.AppendLine(" /******** Configuration ");
            output.AppendLine(" * Class count: " + m_settings.ClassCount.ToString() );
            output.AppendLine(" * Derivation factor: " + m_settings.DerivationFactor.ToString());
            output.AppendLine(" * Virtual table chance: " + ((int)(m_settings.VTableChance * 100)).ToString() + "%");
            output.AppendLine(" * Virtual base chance: " + ((int)(m_settings.VBaseChance * 100)).ToString() + "%");
            output.AppendLine(" * Allow multipe inheritance: " + m_settings.AllowMultipleInheritance.ToString() + "");
            output.AppendLine(" ********/");

            output.AppendLine("namespace reflective");
            output.AppendLine("{");
            output.Tab();

            output.AppendLine("namespace details");
            output.AppendLine("{");
            output.Tab();

            output.AppendLine("namespace " + m_settings.HierarchyName);
            output.AppendLine("{");
            output.Tab();

            foreach ( ClassEntry classEntry in m_classes)
            {
                classEntry.WriteDefinition(output, m_settings.EmitTypeChecks);
            }
            
            output.Untab();
            output.AppendLine("}");

            output.Untab();
            output.AppendLine("}");

            output.AppendLine("");
            foreach (ClassEntry classEntry in m_classes)
            {
                classEntry.WriteReflection(output);
            }
            output.Untab();
            output.AppendLine("};");

            GenerateTest(output);

            return output.ToString();
        }
    }
}
