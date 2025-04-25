const peerConfiguration = {
    iceServers: [ { urls: [ 'stun:stun.l.google.com:19302', 'stun:stun1.l.google.com:19302' ] } ]
}

async function RTCConnect() {
    const peer = new RTCPeerConnection(peerConfiguration);
    const peerSocket = new WebSocket("/RTC_receiver");
    const video = document.querySelector("video");

    const stream = new MediaStream();
    video.srcObject = stream;

    peer.addEventListener("icecandidate", ({ candidate }) => {
        if (candidate) {
            console.log("Local ICE candidate fetched.");
            peerSocket.send(JSON.stringify(["iceCandidate", candidate]));
        }
    });
    
    peerSocket.addEventListener("message", async (msg) => {
        const [eventName, data] = JSON.parse(msg.data);
        
        if (eventName === "offer") {
            console.log("Offer received.");
            await peer.setRemoteDescription(new RTCSessionDescription(data));
            const answer = await peer.createAnswer();
            await peer.setLocalDescription(answer);
            console.log("Sending answer.");
            peerSocket.send(JSON.stringify(["answer", answer]));
        }
        else if (eventName === "iceCandidate") {
            await peer.addIceCandidate(data);
        }
    });
    
    peer.onsignalingstatechange = (e) => console.log(e.target.signalingState);
    
    peer.ontrack = async ({track}) => {
        stream.addTrack(track);
    }
}

window.addEventListener("DOMContentLoaded", RTCConnect);
