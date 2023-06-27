#ifndef QCXUTIL_LETTERMODEL_1687253994913_H
#define QCXUTIL_LETTERMODEL_1687253994913_H
#include "qcxutil/interface.h"
#include <QtCore/QObject>
#include <memory>

#include "topomesh/alg/letter.h"

typedef std::shared_ptr<trimesh::TriMesh> TriMeshPtr;

struct LetterConfigPara
{
	QString dragType="Text";
	int x = 0;
	int y = 0;
	int tShapeW = 0;
	int tShapeH = 0;
	int tRotate = 0;

	QString text = "text";
	QString fontfamily = "";
	QString fontstyle ="";
	int fontsize = 20;
};

namespace qcxutil
{
	class QCXUTIL_API LetterModel : public QObject
	{
		Q_OBJECT
		Q_PROPERTY(float textThickness READ textThickness WRITE setTextThickness NOTIFY textThicknessChanged)
		Q_PROPERTY(bool textSide READ textSide WRITE setTextSide NOTIFY textSideChanged)
	public:
		LetterModel(QObject* parent = nullptr);
		virtual ~LetterModel();

		float textThickness();
		void setTextThickness(float textThickness);
		bool textSide();
		void setTextSide(bool outSide);

		TriMeshPtr _generate(TriMeshPtr mesh,
			const QSize& surfaceSize, const topomesh::SimpleCamera& camera,
			ccglobal::Tracer* tracer = nullptr);
		//解析qml的图元的 坐标等属性，在主线程中调用，在子线程中进行QQmlProperty处理会有偶现崩溃
		void parseQmlData(const QList<QObject*>& objectList);
		void generatePolygons( const QSize& surfaceSize, std::vector<topomesh::TriPolygons>& outPolygon);
		trimesh::fxform calculateTransform(int w, int h);
	signals:
		void textThicknessChanged();
		void textSideChanged();

	protected:
		topomesh::LetterParam m_param;
		QList<QSharedPointer<LetterConfigPara>> m_listLetterConfig;
	};
}

#endif // QCXUTIL_LETTERMODEL_1687253994913_H
