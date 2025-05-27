using System;
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
    public partial class Form3 : Form
    {
        public Form3()
        {
            InitializeComponent();
        }

        private float a = 1, r = 1, g = 1, b = 1;

        public float getA()
        { //For Form1 to get width
            return a;
        }
        public float getR()
        {
            return r;
        }

        public float getG()
        { //For Form1 to get width
            return g;
        }

        public float getB()
        { //For Form1 to get width
            return b;
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            r = (float)trackBar1.Value / 10;
            label5.Text = r.ToString();
        }

        private void trackBar2_Scroll(object sender, EventArgs e)
        {
            g = (float)trackBar2.Value / 10;
            label6.Text = g.ToString();
        }



        private void trackBar3_Scroll(object sender, EventArgs e)
        {
            b = (float)trackBar3.Value / 10;
            label7.Text = b.ToString();
        }

        private void trackBar4_Scroll(object sender, EventArgs e)
        {
            a = (float)trackBar4.Value / 10;
            label8.Text = a.ToString();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
