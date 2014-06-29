#pragma once

#include <Lupus/Network/Enum.h>

namespace Lupus {
    struct SocketInformation;
    class IPEndPoint;
    class IPAddress;

    namespace Internal {
        class SocketState;
    }

    class LUPUS_API Socket : public ReferenceType
    {
    public:
        /*!
         * Erstellt einen neuen Socket anhand von serialisierten Daten eines 
         * anderen Sockets. Falls der zuvor serialisierte Socket bereits
         * verbunden oder gebunden war, dann wird auch dieser Socket
         * automatisch verbunden bzw gebunden.
         *
         * \param[in]   socketInformatoin   Die serialisierten Socketdaten
         */
        Socket(const SocketInformation& socketInformation) throw(std::invalid_argument, socket_error);

        /*!
         * Erstellt einen Socket anhand der Domäne, des zu verwendenden
         * Protokolls und des Sockttyps.
         *
         * \param[in]   family      Die zu verwendende Socketdomäne.
         * \param[in]   type        Der zu verwendende Sockettyp.
         * \param[in]   protocol    Das zu verwendende Protokoll.
         */
        Socket(AddressFamily family, SocketType type, ProtocolType protocol) throw(socket_error);
        virtual ~Socket();

        /*!
         * Akzeptiert eine neue Verbindung falls sich eine im Queue befindet.
         * Diese Funktion ist blockierend und wartet so lange bis ein
         * eingehender Peer sich verbinden will.
         *
         * \returns Zeiger auf den erstellten Socket der Remote-Verbindung.
         */
        virtual Pointer<Socket> Accept() throw (socket_error);

        /*!
         * Bindet diesen Socket an einen lokalen IP-Endpunkt. Diese Methode
         * funktioniert nur für lokale IP-Adressen, da sie den Datenverkehr der
         * Verbindung abfängt. Der Socket hat keinen exklusiven Zugang, was es
         * ermöglicht auch weitere Sockets an diesen Endpunkt zu binden.
         *
         * \param[in]   localEndPoint   Lokaler Endpunkt an den sich der Socket
         *                              binden soll.
         */
        virtual void Bind(Pointer<IPEndPoint> localEndPoint) throw(socket_error);

        /*!
         * Schließt die Socketverbindung.
         */
        virtual void Close() throw(socket_error);

        /*!
         * Schließt die Socketverbindung frühestens nach den angegebenen
         * Sekunden in timeout. Bis dahin sind noch weiterhin alle gültigen
         * Operationen erlaubt.
         *
         * \param[in]   timeout Der Zeitintervall bis zum schließen, angegeben
         *                      in Sekunden.
         */
        virtual void Close(U32 timeout) throw(socket_error);

        /*!
         * Verbindet diesen Socket zu einen Remote-Endpunkt. Sobald die
         * Verbindung aufgebaut ist, können Lese- und Schreiboperationen
         * ausgeführt werden.
         * 
         * \param[in]   remoteEndPoint  Der IP-Endpunkt mit dem sich verbunden
         *                              wird.
         */
        virtual void Connect(Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, null_pointer);

        /*!
         * Ruft Connect(Pointer<IPEndPoint>) auf.
         * \sa Connect(Pointer<IPEndPoint>)
         */
        virtual void Connect(Pointer<IPAddress> address, U16 port) throw(socket_error, null_pointer);

        /*!
         * Versucht eine Verbindung zu einem der angegebenen Endpunkte
         * aufzubauen. Die Endpunkte werden normalerweise durch
         * GetAddressInformation erzeugt. Jedoch darf die Liste jeden
         * beliebigen gültigen Endpunkt enthalten.
         *
         * Der erste Endpunkt mit dem sich erfolgreich Verbunden wurde, beendet
         * den aufruf.
         *
         * \sa GetAddressInformation(const String& node, const String& service, AddressFamily family, SocketType type, ProtocolType protocol)
         *
         * \param[in]   endPoints   Endpunkte mit denen sich Verbunden werden
         *                          soll.
         */
        virtual void Connect(const Vector<Pointer<IPEndPoint>>& endPoints) throw(null_pointer);

        /*!
         * Ruft Connect(Pointer<IPEndPoint>) auf.
         * \sa Connect(Pointer<IPEndPoint>)
         */
        virtual void Connect(const String& host, U16 port) throw(socket_error, std::invalid_argument);

        /*!
         * Serialisiert den Socket und schließt in anschließend. Die
         * serialisierten Daten werden anschließend retouniert.
         *
         * \returns Serialisierte Socketdaten.
         */
        virtual SocketInformation DuplicateAndClose() throw(null_pointer, socket_error);

        /*!
         * Nach dem sich der Socket an eine lokale Adresse gebunden hat, wartet
         * dieser nun auf eingehende Verbindungen.
         * 
         * \param[in]   backlog Die maximale Anzahl an Verbindungen im Queue.
         */
        virtual void Listen(U32 backlog) throw(socket_error);

        /*!
         * Überprüft im angegebenen Zeitintervall den Socket. Gültige Modi sind
         * alle Werte im SocketPollFlags enum. Diese Werte können auch via 
         * OR-Bit verknüpft werden.
         *
         * Das Ergebnis wird dann als OR-Bit retouniert und beinhaltet jenen
         * Wert der als erstes aufgetretten ist.
         *
         * \param[in]   milliSeconds    Der Zeitintervall in dem gewartet
         *                              werden soll.
         * \param[in]   mode            Die Modi auf die gewartet wird.
         *
         * \returns Das Resultat der Operation.
         */
        virtual SocketPollFlags Poll(U32 milliSeconds, SocketPollFlags mode) throw(socket_error);

        /*!
         * Ruft Receive(buffer, 0, buffer.size(), SocketFlags::None, error) 
         * auf.
         *
         * \sa Receive(Vector<Byte>&, U32, U32, SocketFlags, SocketError&)
         */
        virtual S32 Receive(Vector<Byte>& buffer) throw(socket_error);

        /*!
         * Ruft Receive(buffer, offset, buffer.size(), SocketFlags::None, 
         * error) auf.
         *
         * \sa Receive(Vector<Byte>&, U32, U32, SocketFlags, SocketError&)
         */
        virtual S32 Receive(Vector<Byte>& buffer, U32 offset) throw(socket_error, std::out_of_range);

        /*!
         * Ruft Receive(buffer, offset, size, SocketFlags::None, error) auf.
         *
         * \sa Receive(Vector<Byte>&, U32, U32, SocketFlags, SocketError&)
         */
        virtual S32 Receive(Vector<Byte>& buffer, U32 offset, U32 size) throw(socket_error, std::out_of_range);

        /*!
         * Ruft Receive(buffer, offset, size, socketFlats, error) auf.
         *
         * \sa Receive(Vector<Byte>&, U32, U32, SocketFlags, SocketError&)
         */
        virtual S32 Receive(Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags) throw(socket_error, std::out_of_range);

        /*!
         * Liest Daten aus dem Empfangs-Buffer. Diese Methode ist blockiert
         * wenn der Socket so eingestellt ist und liefert einen Fehler wenn 
         * keine Daten vorhanden sind und der Socket auf Non-Blocking 
         * eingestellt ist. Im allgemeinen werden Fehlercodes im errorCode
         * Argument gespeichert.
         *
         * Die Größe des Vektors wird nicht verändert, von daher muss der
         * offset mit dem size Argument in die vorhandene Größe des Vektors
         * passen. Falls die beiden Argumente nicht passen, dann wird eine
         * std::out_of_range Exception geworfen.
         *
         * \param[in,out]   buffer      Der Vektor in dem die Daten gespeichert
         *                              werden.
         * \param[in]       offset      Der offset für den Vektor.
         * \param[in]       size        Die zu lesende Größe.
         * \param[in]       socketFlags Die zu verwendenden Flags.
         * \param[out]      errorCode   Fehlercode im Fehlerfall.
         *
         * \returns Die Anzahl der erhaltenen Bytes. Falls die Verbindung
         *          geschlossen wurde dann Null. Oder einen Fehlercode, wenn
         *          ein Fehler aufgetreten ist.
         */
        virtual S32 Receive(Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, SocketError& errorCode) throw(socket_error, std::out_of_range);

        /*!
         * Ruft ReceiveFrom(buffer, 0, buffer.size(), SocketFlags::None, 
         * remoteEndPoint) auf.
         *
         * \sa ReceiveFrom(Vector<Byte>&, U32, U32, SocketFlags, Pointer<IPEndPoint>&)
         */
        virtual S32 ReceiveFrom(Vector<Byte>& buffer, Pointer<IPEndPoint>& remoteEndPoint) throw(socket_error);

        /*!
         * Ruft ReceiveFrom(buffer, 0, size, SocketFlags::None, remoteEndPoint)
         * auf.
         *
         * \sa ReceiveFrom(Vector<Byte>&, U32, U32, SocketFlags, Pointer<IPEndPoint>&)
         */
        virtual S32 ReceiveFrom(Vector<Byte>& buffer, U32 offset, Pointer<IPEndPoint>& remoteEndPoint) throw(socket_error, std::out_of_range);

        /*!
         * Ruft ReceiveFrom(buffer, 0, size, SocketFlags::None, remoteEndPoint)
         * auf.
         *
         * \sa ReceiveFrom(Vector<Byte>&, U32, U32, SocketFlags, Pointer<IPEndPoint>&)
         */
        virtual S32 ReceiveFrom(Vector<Byte>& buffer, U32 offset, U32 size, Pointer<IPEndPoint>& remoteEndPoint) throw(socket_error, std::out_of_range);

        /*!
         * Liest eingehende Daten aus. Diese Daten können von einen belieben
         * Peer stammen, von daher wird der Endpunkt des Senders im
         * remoteEndPoint Argument gespeichert.
         *
         * Diese Methode blockiert solange bis Daten eingetroffen sind. Es sei
         * denn der Socket ist auf Non-Blocking gestellt, dann wird ein Fehler
         * retouniert.
         *
         * Die Größe des Vektors wird nicht verändert, von daher muss der
         * offset mit dem size Argument in die vorhandene Größe des Vektors
         * passen. Falls die beiden Argumente nicht passen, dann wird eine
         * std::out_of_range Exception geworfen.
         *
         * \param[in,out]   buffer          Der Vektor in dem die Daten 
         *                                  gespeichert werden.
         * \param[in]       offset          Der offset für den Vektor.
         * \param[in]       size            Die zu lesende Größe.
         * \param[in]       socktFlags      Die zu vewendenden Flags.
         * \param[out]      remoteEndPoint  Der sendende Endpunkt.
         *
         * \returns Die Anzahl der erhaltenen Bytes. Falls die Verbindung
         *          geschlossen wurde dann Null. Oder einen Fehlercode, wenn
         *          ein Fehler aufgetreten ist.
         */
        virtual S32 ReceiveFrom(Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, Pointer<IPEndPoint>& remoteEndPoint) throw(socket_error, std::out_of_range);

        /*!
         * Ruft Send(buffer, 0, buffer.size(), SocketFlags::None, error) auf.
         *
         * \sa Send(const Vector<Byte>&, U32, U32, SocketFlags, SocketError&)
         */
        virtual S32 Send(const Vector<Byte>& buffer) throw(socket_error);

        /*!
         * Ruft Send(buffer, offset, buffer.size(), SocketFlags::None, error) 
         * auf.
         *
         * \sa Send(const Vector<Byte>&, U32, U32, SocketFlags, SocketError&)
         */
        virtual S32 Send(const Vector<Byte>& buffer, U32 offset) throw(socket_error, std::out_of_range);

        /*!
         * Ruft Send(buffer, offset, size, SocketFlags::None, error) auf.
         *
         * \sa Send(const Vector<Byte>&, U32, U32, SocketFlags, SocketError&)
         */
        virtual S32 Send(const Vector<Byte>& buffer, U32 offset, U32 size) throw(socket_error, std::out_of_range);

        /*!
         * Ruft Send(buffer, offset, size, socketFlags, error) auf.
         *
         * \sa Send(const Vector<Byte>&, U32, U32, SocketFlags, SocketError&)
         */
        virtual S32 Send(const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags) throw(socket_error, std::out_of_range);

        /*!
         * Schreibt Daten zu dem Verbundenen Endpunkt. Wenn der Socket
         * blockiert, dann wartet diese Methode solange bis die Daten gesendet
         * wurden. Andernfalls wird ein Fehlercode im errorCode Argument
         * gespeichert.
         *
         * \param[in]   buffer      Vektor mit den Daten zum senden.
         * \param[in]   offset      Der offset ab dem gesendet wird.
         * \param[in]   size        Die zu sendende Größe.
         * \param[in]   socketFlags Die zu verwendenden Flags.
         * \param[out]  errorCode   Fehlercode im Fehlerfall.
         *
         * \returns Die Anzahl der erhaltenen Bytes. Falls die Verbindung
         *          geschlossen wurde dann Null. Oder einen Fehlercode, wenn
         *          ein Fehler aufgetreten ist.
         */
        virtual S32 Send(const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, SocketError& errorCode) throw(socket_error, std::out_of_range);

        /*!
         * Ruft SendTo(buffer, 0, buffer.size(), SocketFlags::None,
         * remoteEndPoint) auf.
         *
         * \sa SendTo(const Vector<Byte>&, U32, U32, SocketFlags, Pointer<IPEndPoint>)
         */
        virtual S32 SendTo(const Vector<Byte>& buffer, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error);

        /*!
         * Ruft SendTo(buffer, offset, buffer.size(), SocketFlags::None,
         * remoteEndPoint) auf.
         *
         * \sa SendTo(const Vector<Byte>&, U32, U32, SocketFlags, Pointer<IPEndPoint>)
         */
        virtual S32 SendTo(const Vector<Byte>& buffer, U32 offset, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, std::out_of_range);

        /*!
         * Ruft SendTo(buffer, offset, size, SocketFlags::None remoteEndPoint)
         * auf.
         *
         * \sa SendTo(const Vector<Byte>&, U32, U32, SocketFlags, Pointer<IPEndPoint>)
         */
        virtual S32 SendTo(const Vector<Byte>& buffer, U32 offset, U32 size, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, std::out_of_range);

        /*!
         * Sendet Daten an einen gewissen Endpunkt. Falls die Daten nicht
         * gesendet werden können und der Socket auf Blocking eingestellt ist,
         * dann wartet diese Methode solange bis die Daten gesendet werden
         * können. Andernfalls wird ein Fehler retouniert.
         *
         * \param[in]   buffer          Vektor mit den Daten zum senden.
         * \param[in]   offset          Der offset ab dem gesendet wird.
         * \param[in]   size            Die zu sendende Größe.
         * \param[in]   socketFlags     Die zu verwendenden Flags.
         * \param[out]  remoteEndPoint  Der lesende Endpunkt.
         *
         * \returns Die Anzahl der erhaltenen Bytes. Falls die Verbindung
         *          geschlossen wurde dann Null. Oder einen Fehlercode, wenn
         *          ein Fehler aufgetreten ist.
         */
        virtual S32 SendTo(const Vector<Byte>& buffer, U32 offset, U32 size, SocketFlags socketFlags, Pointer<IPEndPoint> remoteEndPoint) throw(socket_error, std::out_of_range);

        /*!
         * Schließt die Verbindung nicht komplett, sondern lediglich den
         * lesenden oder schreibenden Teil. Es können auch beide gleichzeitig
         * geschlossen werden.
         *
         * \param[in]   how Teil der beendet wird.
         */
        virtual void Shutdown(SocketShutdown how) throw(socket_error);

        ///////////////////////////////////////////////////////////////////////
        /// Modifiers
        ///////////////////////////////////////////////////////////////////////
        virtual SocketHandle Handle() const NOEXCEPT;
        virtual bool IsConnected() const NOEXCEPT;
        virtual bool IsBound() const NOEXCEPT;
        virtual bool IsListening() const throw(socket_error);
        virtual AddressFamily Family() const throw(socket_error);
        virtual ProtocolType Protocol() const throw(socket_error);
        virtual SocketType Type() const throw(socket_error);
        virtual U32 Available() const throw(socket_error);
        virtual bool Blocking() const NOEXCEPT;
        virtual void Blocking(bool) throw(socket_error);
        virtual Pointer<IPEndPoint> LocalEndPoint() const NOEXCEPT;
        virtual Pointer<IPEndPoint> RemoteEndPoint() const NOEXCEPT;
        virtual S32 SendBuffer() const throw(socket_error);
        virtual void SendBuffer(S32) throw(socket_error);
        virtual S32 ReceiveBuffer() const throw(socket_error);
        virtual void ReceiveBuffer(S32) throw(socket_error);
        virtual S32 SendTimeout() const NOEXCEPT;
        virtual void SendTimeout(S32) throw(socket_error);
        virtual S32 ReceiveTimeout() const NOEXCEPT;
        virtual void ReceiveTimeout(S32) throw(socket_error);

        ///////////////////////////////////////////////////////////////////////
        /// Static Methods
        ///////////////////////////////////////////////////////////////////////
        static void Select(const Vector<Pointer<Socket>>& checkRead, const Vector<Pointer<Socket>>& checkWrite, const Vector<Pointer<Socket>>& checkError, U32 microSeconds) throw(socket_error);

    private:

        Socket() = default;

        SocketHandle mHandle = INVALID_SOCKET;
        S32 mSendTime = 0; // Windows support
        S32 mRecvTime = 0; // Windows support
        bool mBlocking = true;
        Pointer<IPEndPoint> mLocal;
        Pointer<IPEndPoint> mRemote;
        bool mBound = false;
        bool mConnected = false;

        Pointer<Internal::SocketState> mState;

        friend Internal::SocketState;
    };

    typedef Pointer<Socket> SocketPtr;
}
