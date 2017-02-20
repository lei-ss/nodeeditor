#pragma once

#include <QtCore/QUuid>
#include <QtWidgets/QGraphicsScene>

#include <unordered_map>
#include <tuple>
#include <memory>
#include <functional>

#include "Connection.hpp"
#include "Export.hpp"
#include "DataModelRegistry.hpp"

namespace QtNodes
{

class NodeDataModel;
class FlowItemInterface;
class Node;
class NodeGraphicsObject;
class ConnectionGraphicsObject;
class NodeStyle;

/// Scene holds connections and nodes.
class NODE_EDITOR_PUBLIC FlowScene
  : public QGraphicsScene
{
  Q_OBJECT
public:

  FlowScene(std::shared_ptr<DataModelRegistry> registry =
              std::make_shared<DataModelRegistry>());

  ~FlowScene();

public:

  std::shared_ptr<Connection>
  createConnection(PortType connectedPort,
                   Node& node,
                   PortIndex portIndex);

  std::shared_ptr<Connection>
  createConnection(Node& nodeIn,
                   PortIndex portIndexIn,
                   Node& nodeOut,
                   PortIndex portIndexOut);

  std::shared_ptr<Connection>
  restoreConnection(QJsonObject const &connectionJson);

  void
  deleteConnection(Connection& connection);

  Node&
  createNode(std::unique_ptr<NodeDataModel> && dataModel);

  Node&
  restoreNode(QJsonObject const& nodeJson);

  void
  removeNode(Node& node);

  DataModelRegistry&
  registry() const;

  void
  setRegistry(std::shared_ptr<DataModelRegistry> registry);

  void
  iterateOverNodes(std::function<void(Node*)> visitor);

public:

  std::unordered_map<QUuid, std::unique_ptr<Node> > const &
  nodes() const;

  std::unordered_map<QUuid, std::shared_ptr<Connection> > const &
  connections() const;

public:

  void
  save() const;

  void
  load();

signals:

  void
  nodeCreated(Node &n);

  void
  nodeDeleted(Node &n);

  void
  connectionCreated(Connection &c);
  void
  connectionDeleted(Connection &c);

  void
  nodeMoved(Node& n, const QPointF& newLocation);

private:

  using SharedConnection = std::shared_ptr<Connection>;
  using UniqueNode       = std::unique_ptr<Node>;

  std::unordered_map<QUuid, SharedConnection> _connections;
  std::unordered_map<QUuid, UniqueNode>       _nodes;
  std::shared_ptr<DataModelRegistry>          _registry;
};

Node*
locateNodeAt(QPointF scenePoint, FlowScene &scene,
             QTransform viewTransform);
}
