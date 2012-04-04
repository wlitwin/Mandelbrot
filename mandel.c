#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include <stdio.h>
#include <complex.h>

#define WIDTH 800
#define HEIGHT 600

#define WCONST (800.0)
#define HCONST (600.0)

int mandelbrot[WIDTH][HEIGHT];

#define EL 6 
unsigned int colortable[] =
{
	0x007F0000,
	0x007F3F00,
	0x007F7F00,
	0x00007F00,
	0x0000007F,
	0x007F007F,
};

int offset = 0;

void build_mandel_diff()
{
	const int LEFT   = -2.0;
	const int RIGHT  =  1.0;
	const int TOP    =  1.0;
	const int BOTTOM = -1.0;

	complex z0 = 0 + 0*I;
	complex power = 2 + 2*I;
	
	for (int yi = 0; yi < HEIGHT; ++yi)
	{
		for (int xi = 0; xi < WIDTH; ++xi)
		{
			complex c = (LEFT + xi * (RIGHT-LEFT)/WCONST) + (TOP + yi * (BOTTOM-TOP)/HCONST)*I;
			complex z  = z0;
			int iter = 0;
			for (; iter < 1000 && cabs(z) < 2; ++iter) 
			{
				z = z*z + c;	
			}
			
			if (iter >= 1000)
			{	
				mandelbrot[xi][yi] = 0;
			} else {
				mandelbrot[xi][yi] = iter % 6;
			}
		}
	}
}

void build_mandel_brot()
{
	double xtemp = 0;
	double y0 = 0;
	double x0 = 0;
	double x  = 0;
	double y  = 0;
	int iteration = 0;
	const int max_iter = 1000;
	const int half_max_iter = max_iter/2;
	
	for (int yi = 0; yi < HEIGHT; ++yi)
	{
		y0 = HCONST * yi - 1;
		for (int xi = 0; xi < WIDTH; ++xi)
		{
			x0 = WCONST * xi - 2.5;
			
			x = 0;
			y = 0;

			iteration = 0;
			while (x*x+y*y < (4) && iteration < max_iter)
			{
				xtemp = x*x - y*y + x0;
				y = 2*x*y + y0;
				x = xtemp;
				++iteration;
			}

			if ((x*x + y*y) < 4)
			{
				mandelbrot[xi][yi] = 0;
			} else {
				unsigned char color = (unsigned char)((double)iteration / (double)max_iter * 255.0);
				mandelbrot[xi][yi] = 0;
				if (iteration > half_max_iter)
				{
					mandelbrot[xi][yi] = (color << 16) | (color << 8) | color;
				} else {
					mandelbrot[xi][yi] = (color << 16) | (color << 8) | color;
				}
			}
		}
	}
}

void initOpenGL()
{
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0,	WIDTH, 0, HEIGHT, -1, 1);
}

void render(void)
{
	unsigned char* val = 0;
	glBegin(GL_POINTS);
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			int index = (mandelbrot[x][y] + offset) % 6;
			val = (unsigned char *)&colortable[index];
			//printf("R: %02X G: %02X B: %02X\n", val[2], val[1], val[0]);
			glColor3b(val[2], val[1], val[0]);
			glVertex2f(x, y);
		}
	}
	glEnd();

	glutSwapBuffers();
}

void timer(int val)
{
	offset = (offset + 1) % EL;
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Mandelbrot");

	glutDisplayFunc(render);
	glutReshapeFunc(reshape);

	initOpenGL();
	//build_mandel_brot();
	build_mandel_diff();
	//glutTimerFunc(16, timer, 0);
	glutMainLoop();
	return 0;
}
