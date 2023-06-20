#ifndef QCXUTIL_LETTERMODEL_1687253994913_H
#define QCXUTIL_LETTERMODEL_1687253994913_H
#include "qcxutil/interface.h"
#include <QtCore/QObject>
#include <memory>

#include "topomesh/alg/letter.h"

typedef std::shared_ptr<trimesh::TriMesh> TriMeshPtr;

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

		TriMeshPtr _generate(TriMeshPtr mesh, const QList<QObject*>& objectList,
			const QSize& surfaceSize, const topomesh::SimpleCamera& camera,
			ccglobal::Tracer* tracer = nullptr);

		void generatePolygons(const QList<QObject*>& objectList, const QSize& surfaceSize, std::vector<topomesh::TriPolygons>& outPolygon);
		trimesh::fxform calculateTransform(int w, int h);
	signals:
		void textThicknessChanged();
		void textSideChanged();

	protected:
		topomesh::LetterParam m_param;
	};
}

#endif // QCXUTIL_LETTERMODEL_1687253994913_H