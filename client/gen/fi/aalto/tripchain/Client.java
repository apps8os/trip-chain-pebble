/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: /Users/loveipeter/trip-chain-pebble/client/src/fi/aalto/tripchain/Client.aidl
 */
package fi.aalto.tripchain;
public interface Client extends android.os.IInterface
{
/** Local-side IPC implementation stub class. */
public static abstract class Stub extends android.os.Binder implements fi.aalto.tripchain.Client
{
private static final java.lang.String DESCRIPTOR = "fi.aalto.tripchain.Client";
/** Construct the stub at attach it to the interface. */
public Stub()
{
this.attachInterface(this, DESCRIPTOR);
}
/**
 * Cast an IBinder object into an fi.aalto.tripchain.Client interface,
 * generating a proxy if needed.
 */
public static fi.aalto.tripchain.Client asInterface(android.os.IBinder obj)
{
if ((obj==null)) {
return null;
}
android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
if (((iin!=null)&&(iin instanceof fi.aalto.tripchain.Client))) {
return ((fi.aalto.tripchain.Client)iin);
}
return new fi.aalto.tripchain.Client.Stub.Proxy(obj);
}
@Override public android.os.IBinder asBinder()
{
return this;
}
@Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
{
switch (code)
{
case INTERFACE_TRANSACTION:
{
reply.writeString(DESCRIPTOR);
return true;
}
case TRANSACTION_onLocation:
{
data.enforceInterface(DESCRIPTOR);
java.util.List<android.location.Location> _arg0;
_arg0 = data.createTypedArrayList(android.location.Location.CREATOR);
this.onLocation(_arg0);
reply.writeNoException();
return true;
}
}
return super.onTransact(code, data, reply, flags);
}
private static class Proxy implements fi.aalto.tripchain.Client
{
private android.os.IBinder mRemote;
Proxy(android.os.IBinder remote)
{
mRemote = remote;
}
@Override public android.os.IBinder asBinder()
{
return mRemote;
}
public java.lang.String getInterfaceDescriptor()
{
return DESCRIPTOR;
}
@Override public void onLocation(java.util.List<android.location.Location> locations) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeTypedList(locations);
mRemote.transact(Stub.TRANSACTION_onLocation, _data, _reply, 0);
_reply.readException();
}
finally {
_reply.recycle();
_data.recycle();
}
}
}
static final int TRANSACTION_onLocation = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
}
public void onLocation(java.util.List<android.location.Location> locations) throws android.os.RemoteException;
}
