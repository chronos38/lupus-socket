#pragma once

#include <Lupus/Definitions.h>

namespace Lupus {
    //! Serialisierungsschnittstelle für Objekte
    class ISerializable
    {
    public:
        virtual ~ISerializable() = default;

        /*!
         * Serialisiert das Objekt und speichert is in einen Byte-Buffer.
         * Diese Methode ist unabhängig von der Architektur oder Plattform.
         * Der Byte-Buffer kann bspw auch über das Netzwerk zu anderen Peers
         * geschickt und deserialisiert werden.
         *
         * Diese Methode retouniert immer einen leeren Buffer wenn das Objekt
         * zustandslos ist.
         *
         * \returns Byte-Buffer des Objekts.
         */
        virtual Vector<Byte> Serialize() const NOEXCEPT = 0;

        /*!
         * Deserialisiert den Byte-Buffer und transformiert ihn in ein gültiges
         * Objekt vom serialisierten Typ. Falls Der Byte-Buffer nicht vom Typ
         * des selben Objekts ist, dann ist das Verhalten undefiniert.
         *
         * Zustandslose Objekte ignorieren diese Methode.
         *
         * \param[in]   buffer  Das Objekt in serialisierter Form.
         */
        virtual void Deserialize(const Vector<Byte>& buffer) throw(std::invalid_argument) = 0;
    };
}
