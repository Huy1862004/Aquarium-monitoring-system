function sendToThingSpeak(field) {
    console.log("ok");
    const apiKey = 'F32PMLJS4WPLWDIZ';  // API Key của bạn từ ThingSpeak
    const url = `https://api.thingspeak.com/update?api_key=${apiKey}&field1=${field}`;
}
function dongho() {
    var time = new Date();
    var gio = time.getHours();
    var phut = time.getMinutes();
    var giay = time.getSeconds();
    if (gio < 10) 
        gio = "0" + gio;
    if (phut < 10) 
        phut = "0" + phut;
    if (giay < 10) 
        giay = "0" + giay;
    document.getElementById("time").innerHTML = gio + ":" + phut + ":" + giay;
    setTimeout("dongho()", 1000);
};
dongho();

let currentDegree = 0; // Biến toàn cục để lưu trữ giá trị hiện tại

function updateCircleProgress(targetDegree) {
    const circles = document.querySelectorAll(".circle");
    const colorDegree = 'red';

    circles.forEach((progress) => {
        let currentDegree = parseFloat(progress.dataset.currentDegree) || 28.0; // Khởi tạo currentDegree với giá trị float
        clearInterval(progress.interval); // Xóa interval trước đó nếu có
        
        progress.interval = setInterval(() => {
            if (currentDegree.toFixed(1) === targetDegree.toFixed(1)) {
                clearInterval(progress.interval);
                return;
            }

            // Kiểm tra xem cần tăng hay giảm
            if (currentDegree < targetDegree) {
                currentDegree += 0.1;
            } else {
                currentDegree -= 0.1;
            }

            // Cập nhật màu nền và giá trị hiển thị
            progress.style.background = `conic-gradient(${colorDegree} ${currentDegree}%, #222 0%)`;
            document.getElementById('nhietdo').style.color = 'white';
            document.getElementById('nhietdo').innerText = `${currentDegree.toFixed(1)}`; // Hiển thị với một chữ số thập phân

            // Cập nhật currentDegree vào dataset để sử dụng lại nếu cần
            progress.dataset.currentDegree = currentDegree;
        }, 50);
    });
}




// Gọi hàm updateCircleProgress với giá trị mới khi cần
function changeTargetDegree(newTarget) {
    currentDegree = 0; // Đặt lại currentDegree khi bắt đầu một chu trình mới
    updateCircleProgress(newTarget);
}



/*--------------------------*/
// Quy đổi độ C sang độ F
// function convertCtoF(celsius) {
//     return (celsius * 9/5) + 32; 
// }

// var dbRef = firebase.database().ref().child('Nhiet do');
// var nhietdoF = document.getElementById('nhietdoF'); // Thẻ để hiển thị độ F
// dbRef.on('value', snap => {
//     var nhietdoC = snap.val(); // Lấy giá trị độ C từ Firebase
//     var fahrenheit = convertCtoF(nhietdoC); // Chuyển đổi sang độ F
//     nhietdoF.innerText = fahrenheit.toFixed(1) + "°F"; // Cập nhật thẻ độ F
// });
src="https://www.gstatic.com/firebasejs/8.2.10/firebase-app.js"
src="https://www.gstatic.com/firebasejs/8.2.10/firebase-database.js"

src="https://www.gstatic.com/firebasejs/8.2.10/firebase-analytics.js"
src="congtac.js"


    var firebaseConfig = {
    apiKey: "AIzaSyCzN40PJU2WbYbGiKDGKhR8HAlEBOLw0lI",
    authDomain: "smartcity-389da.firebaseapp.com",
    databaseURL: "https://smartcity-389da-default-rtdb.firebaseio.com",
    projectId: "smartcity-389da",
    storageBucket: "smartcity-389da.appspot.com",
    messagingSenderId: "68396968002",
    appId: "1:68396968002:web:be5ee1d22bc3f2ab58e5db",
    measurementId: "G-1DEX5YQDVC"
    };
    // Initialize Firebase
    firebase.initializeApp(firebaseConfig);
    firebase.analytics();

var nhietDo = document.getElementById('nhietdo');
var dbRef = firebase.database().ref('/sensor/Nhiet_do');

var mucNuoc = document.getElementById('mucnuoc');
var dbRef2 = firebase.database().ref('/sensor/Muc_nuoc');

var doDuc = document.getElementById('doduc');
var dbRef3 = firebase.database().ref('/sensor/Do_duc');

var lightTension = document.getElementById('cuongdoas');
var dbRef4 = firebase.database().ref('/sensor/Cuong_do_as');


/*-------------------------------------------------------------------*/
// Gửi giá trị lên giao diện
dbRef.on('value', snap => {
nhietDo.innerText = snap.val();
updateCircleProgress(snap.val()); });

dbRef2.on('value', snap => mucNuoc.innerText = snap.val()+' cm');
dbRef3.on('value', snap => doDuc.innerText = snap.val()+ '%');
dbRef4.on('value', snap => lightTension.innerText = snap.val() + 'lux');
/*-------------------------------------------------------------------*/


// công tắc 3 cái ở dưới---------------------------------------

var maychoan = firebase.database().ref("maychoan");
var feed = document.getElementById("feed-image");

// Lắng nghe sự thay đổi từ Firebase
maychoan.on('value', snap => { //khi value của db_fan_ref có sự thay đổi, thực hiện dòng lệnh sau :
    feed_value = snap.val(); // Cập nhật giá trị từ Firebase
    if (feed_value == "1") {
        feed.src = "img/feed_turnon.gif"; 
    } else {
        feed.src = "img/feed_turnoff.png";
    }
}); 

var congtacden = firebase.database().ref('congtacden');
var light = document.getElementById("light-image");
//lắng nghe
congtacden.on('value', snap => { //khi value của db_fan_ref có sự thay đổi, thực hiện dòng lệnh sau :
    light_value = snap.val(); // Cập nhật giá trị từ Firebase
    if (light_value == "1") {
        light.src = "img/lightbulb_turnon.png";
    } else {
        light.src = "img/lightbulb_turnoff.png"; 
    }
}); 

var maybom = firebase.database().ref('maybom');
var motor = document.getElementById("motor-image");
//lắng nghe
maybom.on('value', snap => { //khi value của db_fan_ref có sự thay đổi, thực hiện dòng lệnh sau :
    motor_value = snap.val(); // Cập nhật giá trị từ Firebase
    if (motor_value == "1" || motor_value =="-1" ) { // xét Trang_thai == "1" để thay đổi hình
        motor.src = "img/motor_turnon.gif"; 
    } else {
        motor.src = "img/motor_turnoff.png"; 
    }
}); 

