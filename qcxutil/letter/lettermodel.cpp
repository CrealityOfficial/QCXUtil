#include "lettermodel.h"

#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtCore/QFile>
#include <QtQml/QQmlProperty>
#include <QtCore/QDebug>
#include <QtGui/QMatrix>

#include "mmesh/trimesh/algrithm3d.h"
#include "topomesh/alg/letter.h"
#include "clipperxyz/clipper.hpp"

namespace qcxutil
{
	LetterModel::LetterModel(QObject* parent)
		: QObject(parent)
	{

	}

	LetterModel::~LetterModel()
	{

	}

	float LetterModel::textThickness()
	{
		return m_param.deep;
	}

	void LetterModel::setTextThickness(float textThickness)
	{
		m_param.deep = textThickness;
	}

	bool LetterModel::textSide()
	{
		return m_param.concave;
	}

	void LetterModel::setTextSide(bool outSide)
	{
		m_param.concave = outSide;
	}

	TriMeshPtr LetterModel::_generate(TriMeshPtr mesh,
		const QSize& surfaceSize, const topomesh::SimpleCamera& camera, ccglobal::Tracer* tracer)
	{
		std::vector<topomesh::TriPolygons> polygons;
		generatePolygons(surfaceSize, polygons);

		bool letterOpState = false;
		TriMeshPtr result(topomesh::letter(mesh.get(), camera, m_param, polygons, letterOpState, nullptr, tracer));

		if (!letterOpState)
		{
			result.reset();
		}

		return result;
	}

	void LetterModel::parseQmlData(const QList<QObject*>& objectList)
	{
		m_listLetterConfig.clear();
		for (QObject* object : objectList)
		{
			if (!object)
			{
				continue;
			}
			QVariant tmpDragVariant = QQmlProperty::read(object, "dragType");
			if (tmpDragVariant.isNull() || !tmpDragVariant.isValid())
			{
				qDebug() << "dragType error!";
				continue;
			}
			QSharedPointer<LetterConfigPara> letterParaPtr(new LetterConfigPara());
			QString dragType = tmpDragVariant.toString();
			int x = QQmlProperty::read(object, "x").toInt();
			int y = QQmlProperty::read(object, "y").toInt();
			int tShapeW = QQmlProperty::read(object, "width").toInt();
			int tShapeH = QQmlProperty::read(object, "height").toInt();
			int tRotate = QQmlProperty::read(object, "rotation").toInt();
			letterParaPtr->dragType = dragType;
			letterParaPtr->x = x;
			letterParaPtr->y = y;
			letterParaPtr->tShapeW = tShapeW;
			letterParaPtr->tShapeH = tShapeH;
			letterParaPtr->tRotate = tRotate;
			if ("text" == dragType)
			{
				QString text = QQmlProperty::read(object, "text").toString();
				QString fontfamily = QQmlProperty::read(object, "fontfamily").toString();
				QString fontstyle = QQmlProperty::read(object, "fontstyle").toString();
				int fontsize = QQmlProperty::read(object, "fontsize").toInt();

				letterParaPtr->text = text;
				letterParaPtr->fontfamily = fontfamily;
				letterParaPtr->fontstyle = fontstyle;
				letterParaPtr->fontsize = fontsize;
			}
			m_listLetterConfig.push_back(letterParaPtr);
		}



	}
	void LetterModel::generatePolygons( const QSize& surfaceSize, std::vector<topomesh::TriPolygons>& outPolygon)
	{
		outPolygon.clear();

		auto f = [](const QList<QPolygonF>& polys, ClipperLibXYZ::Paths& pathes) {
			pathes.clear();
			int size = polys.size();
			if (size > 0)
			{
				pathes.resize(size);
				for (int i = 0; i < size; ++i)
				{
					const QPolygonF& poly = polys.at(i);
					ClipperLibXYZ::Path& path = pathes.at(i);

					int count = poly.size();
					for (int j = 0; j < count; ++j)
					{
						const QPointF& point = poly.at(j);
						path.push_back(ClipperLibXYZ::IntPoint((int)(1000.0 * point.x()),
							(int)(1000.0 * point.y()), 0));
					}
				}
			}
		};

		std::vector<ClipperLibXYZ::Paths> pathses;
		//for (QObject* object : objectList)
		for(auto paraPtr : m_listLetterConfig)
		{
			if (!paraPtr)
			{
				continue;
			}

			QPainterPath painterPath;


			QString dragType = paraPtr->dragType;
			int x = paraPtr->x;
			int y = paraPtr->y;
			int tShapeW = paraPtr->tShapeW;
			int tShapeH = paraPtr->tShapeH;
			int tRotate = paraPtr->tRotate;

			ClipperLibXYZ::Paths paths;
			if ("text" == dragType)
			{
				QString text = paraPtr->text;
				QString fontfamily = paraPtr->fontfamily;
				QString fontstyle = paraPtr->fontstyle;
				int fontsize = paraPtr->fontsize;

				QFont font;
				font.setFamily(fontfamily);
				//font.setStyleName(fontstyle);
				font.setPointSize(fontsize);

				QFontMetrics metrics(font);
				int mH = metrics.height();
				int asc = metrics.ascent();
				int py = (tShapeH - mH) / 2 + asc;

				QMatrix t;
				t.translate((qreal)(x + tShapeW / 2), (qreal)(y + tShapeH / 2));
				t.rotate((qreal)tRotate);
				t.translate(-(qreal)(x + tShapeW / 2), -(qreal)(y + tShapeH / 2));
				painterPath.addText((qreal)x, (qreal)(y + py), font, text);
				QList<QPolygonF> polys = painterPath.toSubpathPolygons(t);

				f(polys, paths);

				ClipperLibXYZ::SimplifyPolygons(paths);
			}

			pathses.push_back(paths);
		}

		topomesh::TriPolygons onePolygons;
		for (int i = 0; i < pathses.size(); i++)
		{
			onePolygons.clear();
			const ClipperLibXYZ::Paths onePath = pathses.at(i);
			int count = onePath.size();
			for (int j = 0; j < count; ++j)
			{
				topomesh::TriPolygon poly;
				for (const ClipperLibXYZ::IntPoint& onePoint : onePath.at(j))
				{
					poly.push_back(trimesh::vec3(INT2MM(onePoint.X), INT2MM(onePoint.Y), 0));
				}
				onePolygons.push_back(poly);
			}
			outPolygon.push_back(onePolygons);
		}

		int w = surfaceSize.width();
		int h = surfaceSize.height();
		trimesh::fxform transform = calculateTransform(w, h);

		for (topomesh::TriPolygons& polygons : outPolygon)
			for (topomesh::TriPolygon& polygon : polygons)
				mmesh::applyMatrix2Points(polygon, transform);
	}

	trimesh::fxform LetterModel::calculateTransform(int w, int h)
	{
		trimesh::fxform xf1 = trimesh::fxform::trans(-1.0f, -1.0f, 0.0f);
		trimesh::fxform xf2 = trimesh::fxform::scale(2.0f / (float)w, 2.0f / (float)h, 1.0f);
		trimesh::fxform xfRot = trimesh::fxform::scale(1.0f, -1.0f, 1.0f);
		trimesh::fxform transform = xfRot * xf1 * xf2;
		return transform;
	}
}