﻿using System;
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

            ClassHierarchy h = new ClassHierarchy("test" ,0.5, 0.5, 6);
            h.FileName = System.Windows.Forms.Application.StartupPath + "/../../../../../../test/auto_gen.cpp";

            h.AddRandomBaseClasses(6);
            textBox1.AppendText(h.GenerateSourceCode());
            
            h.Save();
        }
    }
}
