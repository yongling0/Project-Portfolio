﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _1111410_12
{
    public partial class Form2 : Form
    {
        private int width = -1, height = -1;

        public Form2()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            width = int.Parse(textBox1.Text);
            height = int.Parse(textBox2.Text);
            Close();

        }
        public int getWidth()
        { //For Form1 to get width
            return width;
        }
        public int getHeight()
        {
            return height;
        }

    }
}
