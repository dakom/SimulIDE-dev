/***************************************************************************
 *   Copyright (C) 2012 by Santiago González                               *
 *                                                                         *
 ***( see copyright.txt file at root folder )*******************************/

#pragma once

#include <QPainterPath>

#include "component.h"
#include "updatable.h"
#include "e-pin.h"

enum pinState_t{
    undef_state=0,
    driven_low,    // State is open high, but driven low externally
    open_low,
    open_high,
    out_low,
    out_high,
    input_low,
    input_high
};

class Connector;
class LaChannel;

class Pin : public QGraphicsItem, public ePin, public Updatable
{
    Q_INTERFACES(QGraphicsItem)

    public:
        Pin( int angle, const QPoint pos, QString id, int index, Component* parent=0, int length=8 );
        ~Pin();

        enum pinType_t{
            pinNormal=0,
            pinSocket,
            pinHeader,
            pinNull,
            pinRst
        };

        enum { Type = UserType + 3 };
        int type() const override { return Type; }

        // Paint area includes the connection markers drawn around the tip
        QRectF boundingRect() const override { return m_area.united( QRectF(-5,-5, 10, 10 ) ); }
        // Hit area stays the pin itself
        QPainterPath shape() const override { QPainterPath path; path.addRect( m_area ); return path; }

        QString pinId() { return m_id; }
        
        bool unused() { return m_unused; }
        void setUnused( bool unused );

        int length() { return m_length; }
        virtual void setLength( double length );

        void setColor( QColor color ) { m_color[0] = color; }
        void setPinAngle( int angle );
        int pinAngle() { return m_angle; }

        void setX( double x );
        void setY( double y );

        void setBoundingRect( QRect area ) { m_area = area; }
        
        Component* component() { return m_component; }

        Connector* connector() { return my_connector; }
        void setConnector( Connector* c );
        void removeConnector();

        void setConPin( Pin* pin ) { m_conPin = pin; }
        Pin* conPin(){ return m_conPin; }

        Pin* connectPin( bool connect );

        QString getLabelText() { return m_labelText; }
        virtual void setLabelText( QString label, bool over=true );
        void setLabelPos();
        void setLabelColor( QColor color );
        void setFontSize( int size );
        int  labelSizeX() { return m_labelWidth; }

        void setSpace( double s );
        double space() { return m_space; }

        void setPinId( QString id ) { m_id = id; }
        void setVisible( bool visible );

        void moveBy( int dx, int dy );

        void connectorRemoved();

        void registerEnode( eNode* enode, int n=-1 );
        void registerPinsW( eNode* enode, int n=-1 );
        
        void setIsBus( bool bus );
        bool isBus() { return m_isBus; }

        void setPinType( pinType_t ty ) { m_pinType = ty; }
        pinType_t pinType() { return m_pinType; }

        void setDataChannel( LaChannel* ch ) { m_dataCannel = ch; }

        enum conHint_t{     // Feedback while drawing a wire
            hintNone=0,
            hintValid,      // The wire being drawn will connect to this pin
            hintInvalid,    // The mouse is over this pin, but the wire can't connect to it
            hintStart       // The wire being drawn starts at this pin
        };
        void setConHint( conHint_t hint );

        void warning( bool w );
        void animate( bool an );
        virtual void updateStep() override;

        virtual Pin* getPin() override { return this; }

        void isMoved();
        void flip( int h, int v );

    protected:
        void paint( QPainter* p, const QStyleOptionGraphicsItem* o, QWidget* w ) override;
        void mousePressEvent( QGraphicsSceneMouseEvent* event ) override;

        pinType_t  m_pinType;
        pinState_t m_pinState;

        double m_space;
        double m_length;
        int m_angle;
        int m_Hflip;
        int m_Vflip;
        int m_overScore;
        int m_labelheight;
        int m_labelWidth;

        bool m_blocked;
        bool m_isBus;
        bool m_unused;
        bool m_animate;
        bool m_warning;
        bool m_nodePin;     // Pin of a Node (junction): already drawn as a dot

        conHint_t m_conHint;

        double  m_opCount;

        QString m_labelText;

        uint32_t* m_foreColor;
        QColor m_color[8];
        QRectF     m_area;
        Connector* my_connector;
        Component* m_component;
        LaChannel* m_dataCannel;    // connect to Logic Analyzer
        Pin*       m_conPin;        // Pin at the other side of connector

        QGraphicsSimpleTextItem m_label;
};
