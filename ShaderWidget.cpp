#include "ShaderWidget.h"
#include <QFileInfo>
ShaderWidget::ShaderWidget(QWidget *parent) : QOpenGLWidget(parent), m_position(0) {
	vert_data = new GLfloat[12];
	// x y z
	vert_data[0] = -1.0f; vert_data[1] = -1.0f; vert_data[2] = 0.0f;
	vert_data[3] = 1.0f;  vert_data[4] = -1.0f; vert_data[5] = 0.0f;
	vert_data[6] = 1.0f;  vert_data[7] = 1.0f;  vert_data[8] = 0.0f;
	vert_data[9] = -1.0f; vert_data[10] = 1.0f; vert_data[11] = 0.0f;
}

ShaderWidget::~ShaderWidget() {
	delete[] vert_data;
}
// код, компилирующий шейдеры и компонующий из них шейдерную программу
void ShaderWidget::initializeGL() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	QOpenGLShader vShader(QOpenGLShader::Vertex);//Класс QOpenGLShader умеет загружать исходный код шейдера с диска компилировать его
    QFileInfo info("raytracing.vert");
    qDebug() << info.absoluteFilePath();
	vShader.compileSourceFile("raytracing.vert");
	QOpenGLShader fShader(QOpenGLShader::Fragment);
	fShader.compileSourceFile("raytracing.frag");
	m_program.addShader(&vShader);
    m_program.addShader(&fShader);//компонует добавленные шейдеры в шейдерную программу
    if (!m_program.link()) {
		qWarning("Error link");
		return;
	}
	m_position = m_program.attributeLocation("vertex");// получаем расположение нашего массива вершин в пределах списка параметров шейдерной программы
	//qDebug() << QString("Log programm");
	//qDebug() << m_program.log();
	
	if (!m_program.bind()) {
		qWarning("error bind programm shader");
		return;
	}
	// начальные значения камеры 
	m_program.setUniformValue("camera.position", QVector3D(0.0, 0.0, -10));
	m_program.setUniformValue("camera.view", QVector3D(0.0, 0.0, 1.0));
	m_program.setUniformValue("camera.up", QVector3D(0.0, 1.0, 0.0));
	m_program.setUniformValue("camera.side", QVector3D(1.0, 0.0, 0.0));

	m_program.setUniformValue("scale", QVector2D(width(), height()));

	m_program.release();

	std::vector<Sphere> all_spheres;// массив сфер, которые вы хотите отрисовать
	//расположение сфер относительно положения наблюдателя (камеры)
	//all_spheres.push_back(Sphere{ QVector3D(-1, 0, -2), 2, QVector3D(1.0, 0.4, 0), 0 });
	//all_spheres.push_back(Sphere{ QVector3D(2, 0, -3), 1, QVector3D(0.0, 0.6, 0.2), 0 });
    all_spheres.push_back(Sphere{ QVector3D(-4, 1, 0), 2, QVector3D(0.5, 0.1, 0.75), 0 });
    all_spheres.push_back(Sphere{ QVector3D(0, 0, -3), 1.5, QVector3D(0.05, 0.4, 0.8), 0 });
    all_spheres.push_back(Sphere{ QVector3D(1, 3, 6), 2, QVector3D(0.75, 0.0, 0.0), 0 });

	//Для того, чтобы передать массив на видеокарту воспользуемся SSBO
	//Создаём экземпляр класса QOpenGLFunctions_4_3_Core для того, чтобы получить доступ к нужным функциям
	functions = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
	functions->glGenBuffers(1, &ssbo);
	functions->glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	functions->glBufferData(GL_SHADER_STORAGE_BUFFER, all_spheres.size() * sizeof(Sphere), all_spheres.data(), GL_DYNAMIC_COPY);
	functions->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

}

void ShaderWidget::resizeGL(int nWidth, int nHeight) {
	glViewport(0, 0, nWidth, nHeight);
	if (!m_program.bind()) {
		qWarning("error bind programm shader");
	}
	m_program.setUniformValue("scale", QVector2D(width(), height()));
	//qDebug() << "scale = " << QVector2D(width(), height());
	m_program.release();
}

void ShaderWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);
	if (!m_program.bind())
	{
		return;
	}
	m_program.enableAttributeArray(m_position);
	m_program.setAttributeArray(m_position, vert_data, 3);
	glDrawArrays(GL_QUADS, 0, 4);// отвечает за рисование примитивов
	//первый аргумент - тип примитива, второй-с вершины под каким номером нужно начать, третий-задаёт кол-во вершин, кот-е следует использовать
	m_program.disableAttributeArray(m_position);
	m_program.release();
}

