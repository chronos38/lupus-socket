#pragma once

#include <Lupus/Network/Enum.h>

namespace Lupus {
    class IPEndPoint;

    //! Repräsentiert einen IP-Adresse
    class LUPUS_API IPAddress : public ReferenceType
    {
    public:
        //! Standardkonstruktor ist nicht erlaubt.
        IPAddress() = delete;

        /*!
         * Erstellt eine neue IP-Adresse anhand der übergenenen IPv4 Adresse.
         * Das Format der Ganzzahl ist 0xSSTTUUVV -> sss.ttt.uuu.vvv und sie
         * wird in das entsrechende Netzwerkformat konvertiert.
         *
         * \param[in]   ipv4    Ganzzahl die eine IPv4 Adresse beinhaltet.
         */
        IPAddress(U32 ipv4) NOEXCEPT;

        /*!
         * Dieser Konstruktor ruft IPAddress(address, 0) auf.
         *
         * \sa IPAddress::IPAddress(const Vector<Byte>&, U64)
         *
         * \param[in]   ipv6    Die IPv6 Adresse in Netzwerkformat.
         */
        IPAddress(const Vector<Byte>& ipv6) throw(std::length_error);

        /*!
         * Erstellt eine IP-Adresse anhand eines Byte-Buffers. Der Byte-Buffer
         * muss exakt 16-Byte bzw 128-Bit umfassen. Falls der Byte-Buffer
         * größer ist, dann werden die restlichen Bytes nach dem 16ten
         * ignoriert.
         *
         * Die Adresse muss sich Big-Endian sein.
         *
         * \param[in]   ipv6    Die IPv6 Adresse in Netzwerkformat.
         * \param[in]   scopeid Der Scope Identifier der IPv6 Adresse.
         */
        IPAddress(const Vector<Byte>& ipv6, U32 scopeid) throw(std::length_error);
        virtual ~IPAddress() = default;

        /*!
         * Serialisiert die Adresse zu einem Byte-Buffer.
         *
         * \returns Byte-Buffer der serialisierten Adresse.
         */
        virtual Vector<Byte> AddressBytes() const NOEXCEPT;

        /*!
         * \returns Die Adressfamilie der IP-Adresse.
         */
        virtual AddressFamily Family() const NOEXCEPT;

        /*!
         * \warning Methode ist noch nicht implementiert.
         */
        virtual bool IsIPv6LinkLocal() const NOEXCEPT;

        /*!
         * \warning Methode ist noch nicht implementiert.
         */
        virtual bool IsIPv6Multicast() const NOEXCEPT;

        /*!
         * \warning Methode ist noch nicht implementiert.
         */
        virtual bool IsIPv6SiteLocal() const NOEXCEPT;

        /*!
         * \returns Den Scope Identifier der IPv6 Adresse.
         */
        virtual U32 ScopeId() const throw(socket_error);

        /*!
         * Setzt den Scope Identifier der IPv6 Adresse.
         *
         * \param[in]   value   Der neue Wert des Scope Identifiers.
         */
        virtual void ScopeId(U32 value) throw(socket_error);

        /*!
         * \warning Methode ist noch nicht implementiert.
         */
        static bool IsLoopback(Pointer<IPAddress> address) NOEXCEPT;

        /*!
         * Erstellt eine IP-Adresse anhand des gegebenen Präsentationsformat.
         * IPv4 muss in dezimal und IPv6 in hexadezimal dargestellt werden.
         *
         * Bsp IPv4: 127.0.0.1
         * Bsp IPv6: 0:0:0:0:0:0:0:1
         *
         * \param[in]   ipString    Das Präsentationsformat der IP-Adresse.
         */
        static Pointer<IPAddress> Parse(const String& ipString) throw(std::invalid_argument);

        /*!
         * Ähnlich wie \sa IPAddress::Parse konvertiert diese Methode eine 
         * IP-Zeichenkette. Jedoch ist diese Methode Exception-Safe. Falls die
         * Konvertierung dennoch fehlschlägt dann wird FALSE retouniert. Das
         * Ergebniss wird in den Adresszeiger gespeichert.
         *
         * \param[in]   ipString    Das Präsentationsformat der IP-Adresse.
         * \param[out]  address     Zeiger in dem das Ergebnis gespeichert
         *                          wird.
         * 
         * \returns TRUE wenn erfolgreich konvertiert wurde, bei einem Fehler
         *          FALSE.
         */
        static bool TryParse(const String& ipString, Pointer<IPAddress>& address) NOEXCEPT;

        static const Pointer<IPAddress> Any; //!< Entspricht 0.0.0.0
        static const Pointer<IPAddress> Broadcast; //!< Entspricht 255.255.255.255
        static const Pointer<IPAddress> IPv6Any; //!< Entspricht 0:0:0:0:0:0:0:0
        static const Pointer<IPAddress> IPv6Loopback; //!< Entspricht ::1
        static const Pointer<IPAddress> IPv6None; //!< Entspricht 0:0:0:0:0:0:0:0
        static const Pointer<IPAddress> Loopback; //!< Entspricht 127.0.0.1
        static const Pointer<IPAddress> None; //!< Entspricht 0.0.0.0

    protected:

    private:

        AddressFamily mFamily;
        Vector<Byte> mAddress;
        U32 mScopeId;
    };

    typedef Pointer<IPAddress> IPAddressPtr;
}
