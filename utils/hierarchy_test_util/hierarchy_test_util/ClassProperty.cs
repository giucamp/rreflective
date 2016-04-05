using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace hierarchy_test_util
{
    class PropertyFactory
    {
        private Random m_random;
        private List<string> m_nakedTypes = new List<string>();
        private List<string> m_shuffledNakedTypes = new List<string>();
        private int m_propProgr = 0;
        private int m_nextNaked = 0;

        private void cartMulPostfix(List<string> i_dest,  params string [] i_strings )
        {
            List<string> orig = new List<string>(i_dest);
            i_dest.Clear();
            foreach (string str1 in orig)
            {
                foreach (string str2 in i_strings)
                {
                    i_dest.Add(str1 + str2);
                }
            }           
        }

        private void cartMulPreFix(List<string> i_dest, params string[] i_strings)
        {
            List<string> orig = new List<string>(i_dest);
            i_dest.Clear();
            foreach (string str1 in orig)
            {
                foreach (string str2 in i_strings)
                {
                    i_dest.Add(str2 + str1);
                }
            }
        }

        public PropertyFactory(Random i_random)
        {
            m_random = i_random;
            List<string> ints;

            ints = new List < string >( new string[]{ "int" });
            cartMulPreFix(ints, "", "short ", "long ", "long long ");
            cartMulPreFix(ints, "unsigned ", "signed ");
            m_nakedTypes.AddRange( ints );

            ints = new List<string>(new string[] { "char" });
            cartMulPreFix(ints, "", "unsigned ", "signed ");
            m_nakedTypes.AddRange(ints);
            
            m_nakedTypes.AddRange(new string[] { "float", "double", "long double" });
        }
        
        public string PickNakedType()
        {
            if (m_nextNaked >= m_shuffledNakedTypes.Count)
            {
                m_shuffledNakedTypes = new List<string>(m_nakedTypes);
                Utils.Shuffle(m_nakedTypes, m_random);
                m_nextNaked = 0;
            }

            var res = m_shuffledNakedTypes[m_nextNaked];
            m_nextNaked++;
            return res;
        }

        public string PickType(double i_indirectionChance, double i_cvQuaificationChance, int i_maxIndirections)
        {
            string res = PickNakedType();
            int currInd = 0;
            while(currInd + 1 <= i_maxIndirections)
            {
                if (m_random.NextDouble() < i_cvQuaificationChance)
                {
                    res += " volatile";
                }

                if (m_random.NextDouble() < i_cvQuaificationChance)
                {
                    res += " const";
                }

                if (m_random.NextDouble() >= i_indirectionChance)
                    break;

                res += "*";
                currInd++;
            }
            return res;
        }

        public ClassProperty NewProp(double i_indirectionChance, double i_cvQuaificationChance, int i_maxIndirections)
        {
            m_propProgr++;
            ClassProperty res = new ClassProperty(PickType(i_indirectionChance, i_cvQuaificationChance, i_maxIndirections), "m_prop" + m_propProgr);
            return res;
        }
    }

    class ClassProperty
    {
        private string m_type, m_name;

        public ClassProperty(string i_type, string i_name)
        {
            m_type = i_type;
            m_name = i_name;
        }

        public string Type
        {
            get { return m_type; }
        }

        public string Name
        {
            get { return m_name; }
        }
    }
}
