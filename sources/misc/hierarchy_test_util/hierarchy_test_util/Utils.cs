using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace hierarchy_test_util
{
    public static class Utils
    {
        public static void Shuffle<T>(IList<T> i_dest, Random i_rand)
        {
            for (int i = 0; i < i_dest.Count; i++)
            {
                int j = i_rand.Next(i_dest.Count);
                var temp = i_dest[i];
                i_dest[i] = i_dest[j];
                i_dest[j] = temp;
            }
        }

        public static void Shuffle<T>(T[] i_dest, Random i_rand)
        {
            for (int i = 0; i < i_dest.Length; i++)
            {
                int j = i_rand.Next(i_dest.Length);
                var temp = i_dest[i];
                i_dest[i] = i_dest[j];
                i_dest[j] = temp;
            }
        }

        public static int[] CreateIntArray(int i_count)
        {
            int[] res = new int[i_count];
            for(int i = 0; i < i_count; i++)
            {
                res[i] = i;
            }
            return res;
        }

        public static void Swap<T>(ref T i_first, ref T i_second)
        {
            T temp = i_first;
            i_first = i_second;
            i_second = temp;
        }

        public static void RemoveDupicates<T>(this List<T> i_list)
        {
            int count = i_list.Count;
            for(int i = 0; i < count - 1; i++ )
            {
                T el = i_list[i];
                for (int j = i + 1; j < count; j++)
                {
                    if( i_list[j].Equals(el) )
                    {
                        i_list.RemoveAt(j);
                        count--;
                    }
                }                    
            }
        }

        public static bool HasDupicates<T>(this IEnumerable<T> i_collection)
        {
            HashSet<T> set = new HashSet<T>();
            foreach( T el in i_collection)
            {
                if( set.Contains(el))
                {
                    return true;
                }
                else
                {
                    set.Add(el);
                }
            }
            return false;
        }
    }
}
