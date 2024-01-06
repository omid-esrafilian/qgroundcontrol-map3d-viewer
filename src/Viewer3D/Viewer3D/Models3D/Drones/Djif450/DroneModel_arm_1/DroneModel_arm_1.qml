import QtQuick 2.9
import QtQuick3D

///     @author Omid Esrafilian <esrafilian.omid@gmail.com>

Node {
    id: node

    // Resources

    // Nodes:
    Node {
        id: _STL_BINARY_
        Model {
            id: model
            source: "node.mesh"
            materials: [
                PrincipledMaterial {
                    baseColor: "white"
                    metalness: 0.2
                    roughness: 0.1
                    specularAmount: 1.0
                    indexOfRefraction: 1.0
                    opacity: 1.0
                }
            ]
        }
    }

    // Animations:
}
