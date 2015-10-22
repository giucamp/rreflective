using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace hierarchy_test_util
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            ClassHierarchy h = new ClassHierarchy("test",100);
            h.AddRandomBaseClasses(200);
            textBox1.AppendText(h.GetCode());
        }
    }
}
