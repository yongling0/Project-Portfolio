using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing.Imaging;

namespace _1111410_hw6
{
    public partial class Form1 : Form
    {
        Random rd = new Random(DateTime.Now.Millisecond + 1);
        Image bowl = Properties.Resources.Bowl;
        List<Image> fruit = new List<Image>(new Image[] { Properties.Resources.Banana, Properties.Resources.StawBerry, Properties.Resources.Tomato });
        List<Image> background = new List<Image>(new Image[] { Properties.Resources.Hydrangeas, Properties.Resources.Penguins, Properties.Resources.Tulips });
        Point[] fruitpt = new Point[] { new Point(200, 10), new Point(300, 8), new Point(168, 4) };

        int timecount = 120;
        int changebackground = 0;

        int score = 0;

        Point bowlpt = new Point(256, 384);
        public Form1()
        {
            InitializeComponent();
            DoubleBuffered = true;
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            label5.Text = score.ToString();
            float[][] cmArray =
               {
                  new float[] {1, 0, 0, 0,    0},
                  new float[] {0, 1, 0, 0,    0},
                  new float[] {0, 0, 1, 0,    0},
                  new float[] {0, 0, 0, 0.5f, 0}, // 
                  new float[] {0, 0, 0, 0,    1}
               }; // 5 x 5 的矩陣
            ColorMatrix cm = new ColorMatrix(cmArray); // 色彩調整矩陣
            ImageAttributes ia = new ImageAttributes();  // 影像屬性
            ia.SetColorMatrix(cm, ColorMatrixFlag.Default, ColorAdjustType.Bitmap); //設定影像屬性的色彩調整矩陣                                                                                    
            e.Graphics.DrawImage(background[changebackground], new Rectangle(0, 0, background[changebackground].Width / 2, background[changebackground].Height / 2), 0, 0, background[changebackground].Width, background[changebackground].Height, GraphicsUnit.Pixel, ia); // 使用 影像屬性 繪製// 繪出透明的影像
            e.Graphics.DrawImage(bowl, new Rectangle(bowlpt.X, bowlpt.Y, bowl.Width, bowl.Height), 0, 0, bowl.Width, bowl.Height, GraphicsUnit.Pixel);

            for (int i = 0; i < 3; i++)
            {
                e.Graphics.DrawImage(fruit[i], new Rectangle(fruitpt[i].X, fruitpt[i].Y, fruit[i].Width, fruit[i].Height), 0, 0, fruit[i].Width, fruit[i].Height, GraphicsUnit.Pixel);
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            timecount--;
            label2.Text = timecount.ToString() + " Seconds";
            if (timecount == 0)
            {
                timer1.Stop();
                timer2.Stop();
                timer3.Stop();
            }
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            changebackground++;
            Invalidate();
            if (changebackground == 3)
            {
                changebackground = 0;
            }
        }

        private void Form1_MouseMove(object sender, MouseEventArgs e)
        {
            bowlpt.X = e.X;
            if (bowlpt.X < 0)
            {
                bowlpt.X = 0;
            }
            if (bowlpt.X > 442)
            {
                bowlpt.X = 442;
            }
        }

        private void timer3_Tick(object sender, EventArgs e)
        {
            for (int i = 0; i < 3; i++)
            {
                if (fruitpt[i].Y == 364)
                {
                    if (fruitpt[i].X >= bowlpt.X && fruitpt[i].X <= bowlpt.X + 70)
                    {
                        score++;
                    }
                }
            }

            if (fruitpt[0].Y == 364)
            {
                fruitpt[0] = new Point(rd.Next(0, 470), rd.Next(6, 100));
            }

            if (fruitpt[1].Y == 364)
            {
                fruitpt[1] = new Point(rd.Next(0, 470), rd.Next(6, 100));
            }

            if (fruitpt[2].Y == 364)
            {
                fruitpt[2] = new Point(rd.Next(0, 470), rd.Next(6, 100));
            }



            for (int i = 0; i < 3; i++)
            {
                fruitpt[i].Y++; ;
            }

            Invalidate();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            for (int i = 0; i < 3; i++)
            {
                fruitpt[i] = new Point(rd.Next(0, 470), rd.Next(6, 100));
            }

            Size = new Size(600, 800);
            timecount = 120;
            label2.Text = timecount.ToString() + " Seconds";
            score = 0;

            bowlpt = new Point(256, 384);
            timer1.Start();
            timer2.Start();
            timer3.Start();
        }

        private void restartToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Invalidate();
            timer1.Stop();
            timer2.Stop();
            timer3.Stop();
            Form1_Load(sender, e);
        }
    }
}
