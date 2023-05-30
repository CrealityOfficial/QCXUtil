#include "nest2djob.h"

#include "qcxutil/trimesh2/conv.h"

#include "nestplacer/nestplacer.h"

namespace qcxutil
{
    Nest2DJob::Nest2DJob(QObject* parent)
        : Job(parent)
        , m_insert(nullptr)
        , m_nestType(NestPlaceType::CENTER_TO_SIDE)
    {

    }

    Nest2DJob::~Nest2DJob()
    {

    }

    void Nest2DJob::setBounding(const trimesh::box3& box)
    {
        m_box = box;
    }

    void Nest2DJob::setNestType(NestPlaceType type)
    {
        m_nestType = type;
    }

    void Nest2DJob::setInsertItem(PlaceItem* item)
    {
        m_insert = item;
    }

    void Nest2DJob::setItems(const QList<PlaceItem*>& items)
    {
        m_items = items;
    }

    QString Nest2DJob::name()
    {
        return "qcxutil::Nest2DJob";
    }

    QString Nest2DJob::description()
    {
        return "qcxutil::Nest2DJob";
    }

    void Nest2DJob::failed()
    {
    }

    bool caseInsensitiveLessThan(PlaceItem* item1, PlaceItem* item2)
    {
        trimesh::box3 b1 = item1->globalBox();
        trimesh::box3 b2 = item2->globalBox();
        return  b1.size().z > b2.size().z;
    }

    // invoke from main thread
    void Nest2DJob::successed(qtuser_core::Progressor* progressor)
    {
    }

    void Nest2DJob::work(qtuser_core::Progressor* progressor)
    {
        std::sort(m_items.begin(), m_items.end(), caseInsensitiveLessThan);

        if (m_insert)
            insert();
        else
        {
            layoutAll();
        }
    }

    void Nest2DJob::insert()
    {
    }

    void Nest2DJob::layoutAll()
    {
        if (m_items.size() == 0)
            return;

        std::vector<int> modelIndices;
        for (int i = 0; i < m_items.size(); i++)
        {
            modelIndices.push_back(i);
        }
        trimesh::box3 workspaceBox = m_box;
        std::vector < std::vector<trimesh::vec3>> modelsData;
        for (int m = 0; m < m_items.size(); m++)
        {
            std::vector<trimesh::vec3> oItem;
            if (nestplacer::PlaceType(m_nestType) == nestplacer::PlaceType::CONCAVE || 
                nestplacer::PlaceType(m_nestType) == nestplacer::PlaceType::CONCAVE_ALL)
                oItem = m_items.at(m)->outLine_concave();
            else
                oItem = m_items.at(m)->outLine();
            modelsData.push_back(oItem);
        }

        std::function<void(int, trimesh::vec3)> modelPositionUpdateFunc_nest = [this](int modelIndex, trimesh::vec3 newBoxCenter) {
            NestResult result;
            result.x = newBoxCenter.x * NEST_FACTOR;
            result.y = newBoxCenter.y * NEST_FACTOR;
            result.r = newBoxCenter.z;
            result.index = modelIndex;
            m_result.push_back(result);
        };
        nestplacer::NestParaFloat para = nestplacer::NestParaFloat(workspaceBox, 10.0f, nestplacer::PlaceType(m_nestType), true);

        nestplacer::NestPlacer::layout_all_nest(modelsData, modelIndices, para, modelPositionUpdateFunc_nest);
    }
}