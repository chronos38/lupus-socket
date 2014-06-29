#pragma once

#include <Lupus/Network/Enum.h>

namespace Lupus {
    class IPAddress;

    //! Repräsentiert einen Endpunkt mit dem Kommuniziert werden kann.
    class LUPUS_API IPEndPoint : public ReferenceType
    {
    public:
        /*!
         * Erstellt einen IP-Endpunkt mit der angegebenen IPv4 Adresse und
         * bindet diesen an den angebenen Port. Es werden nur IPv4 Adressen
         * unterstützt und diese muss sich in der Form 0xSSTTUUVV ->
         * sss.ttt.uuu.vvv befinden.
         *
         * \param[in]   address Eine valide IPv4 Adresse.
         * \param[in]   port    Die Portnummer.
         */
        IPEndPoint(U32 address, U16 port);

        /*!
         * Erstellt einen IP-Endpunkt mit der angebenen IP-Adresse und bindet
         * diesen an den angebenen Port. Diese Instanz übernimmt den Besitz
         * der IP-Adresse.
         *
         * \param[in]   address Eine gültige IP-Adresse für diesen Endpunkt.
         * \param[in]   port    Die Portnummer.
         */
        IPEndPoint(Pointer<IPAddress> address, U16 port) throw(null_pointer);

        /*!
         * Erstellt einen IP-Endpunkt anhand von serialisierten Daten.
         *
         * \param[in]   buffer  Serialisierte Daten.
         */
        IPEndPoint(const Vector<Byte>& buffer) throw(std::invalid_argument);
        virtual ~IPEndPoint() = default;

        /*!
         * \returns Die Adressfamilie des Endpunkts.
         */
        virtual AddressFamily Family() const NOEXCEPT;

        /*!
         * \returns Die IP-Adresse des Endpunkts.
         */
        virtual Pointer<IPAddress> Address() const NOEXCEPT;
        /*!
         * Setzt die IP-Adresse des Endpunkts.
         *
         * \param[in]   address Eine gültige IP-Adresse.
         */
        virtual void Address(Pointer<IPAddress> address) throw(null_pointer);

        /*!
         * \returns Die Portnummer des Endpunkts.
         */
        virtual U16 Port() const NOEXCEPT;

        /*!
         * Setzt den Port des Endpunkts.
         *
         * \param[in]   port    Der neue Port.
         */
        virtual void Port(U16 port) NOEXCEPT;

        /*!
         * Serialisiert die internen Daten und speichert sie in einen 
         * Byte-Buffer. Der Buffer ist Plattformabhängig, was auch der Grund
         * ist wieso keine ISerializable implementiert wird.
         *
         * \returns Interne Daten in Form eines Byte-Buffers.
         */
        virtual Vector<Byte> Serialize() const NOEXCEPT;

    private:

        IPEndPoint() = default;

        AddrStorage mAddrStorage;
        Pointer<IPAddress> mAddress;
    };

    typedef Pointer<IPEndPoint> IPEndPointPtr;
}
