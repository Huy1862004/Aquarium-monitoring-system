// Khởi tạo Firebase
firebase.initializeApp({
    apiKey: "AIzaSyCzN40PJU2WbYbGiKDGKhR8HAlEBOLw0lI",
    authDomain: "smartcity-389da.firebaseapp.com",
    databaseURL: "https://smartcity-389da-default-rtdb.firebaseio.com",
    projectId: "smartcity-389da",
    storageBucket: "smartcity-389da.appspot.com",
    messagingSenderId: "68396968002",
    appId: "1:68396968002:web:be5ee1d22bc3f2ab58e5db",
    measurementId: "G-1DEX5YQDVC"
});



// Lấy phần tử canvas và khởi tạo biểu đồ
const ctx = document.getElementById('mucNuocChart').getContext('2d');
let mucNuocData = JSON.parse(localStorage.getItem('mucNuocData')) || [];
let timeLabels = JSON.parse(localStorage.getItem('timeLabels')) || [];
const MAX_DATA_POINTS = 10; // Giới hạn số lượng điểm dữ liệu

const mucNuocChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: timeLabels,
        datasets: [{
            label: 'Mực Nước',
            data: mucNuocData,
            borderColor: 'blue',
            borderWidth: 2,
        }]
    },
    options: { scales: { y: { beginAtZero: true } } }
});

// Cập nhật biểu đồ từ Firebase
firebase.database().ref('/sensor/Muc_nuoc').on('value', (snap) => {
    const mucNuocValue = snap.val();
    const currentTime = new Date().toLocaleTimeString();

    // Thêm dữ liệu mới vào mảng
    mucNuocData.push(mucNuocValue);
    timeLabels.push(currentTime);

    // Giới hạn số lượng điểm dữ liệu
    if (mucNuocData.length > MAX_DATA_POINTS) {
        mucNuocData.shift(); // Xóa phần tử đầu tiên
        timeLabels.shift(); // Xóa nhãn thời gian đầu tiên
    }

    mucNuocChart.update();

    localStorage.setItem('mucNuocData', JSON.stringify(mucNuocData));
    localStorage.setItem('timeLabels', JSON.stringify(timeLabels));
});

//----------------------------------------------------------------------------------//

// Lấy phần tử canvas và khởi tạo biểu đồ
const dtx = document.getElementById('nhietDoChart').getContext('2d');
let nhietDoData = JSON.parse(localStorage.getItem('nhietDoData')) || [];
let timeLabels2 = JSON.parse(localStorage.getItem('timeLabels2')) || [];
const MAX_DATA_POINTS2 = 10; // Giới hạn số lượng điểm dữ liệu

const nhietDoChart = new Chart(dtx, {
    type: 'line',
    data: {
        labels: timeLabels2,
        datasets: [{
            label: 'Nhiệt độ',
            data: nhietDoData,
            borderColor: 'red',
            borderWidth: 2,
        }]
    },
    options: { scales: { y: { beginAtZero: true } } }
});

// Cập nhật biểu đồ từ Firebase
firebase.database().ref('/sensor/Nhiet_do').on('value', (snap) => {
    const nhietDoValue = snap.val();
    const currentTime2 = new Date().toLocaleTimeString();

    // Thêm dữ liệu mới vào mảng
    nhietDoData.push(nhietDoValue);
    timeLabels2.push(currentTime2);

    // Giới hạn số lượng điểm dữ liệu
    if (nhietDoData.length > MAX_DATA_POINTS2) {
        nhietDoData.shift(); // Xóa phần tử đầu tiên
        timeLabels2.shift(); // Xóa nhãn thời gian đầu tiên
    }

    nhietDoChart.update();

    localStorage.setItem('nhietDoData', JSON.stringify(nhietDoData));
    localStorage.setItem('timeLabels2', JSON.stringify(timeLabels2));
});

//----------------------------------------------------------------------------------//

// Lấy phần tử canvas và khởi tạo biểu đồ
const etx = document.getElementById('ducNuocChart').getContext('2d');
let ducNuocData = JSON.parse(localStorage.getItem('ducNuocData')) || [];
let timeLabels3 = JSON.parse(localStorage.getItem('timeLabels3')) || [];
const MAX_DATA_POINTS3 = 10; // Giới hạn số lượng điểm dữ liệu

const ducNuocChart = new Chart(etx, {
    type: 'line',
    data: {
        labels: timeLabels3,
        datasets: [{
            label: 'Độ đục nước',
            data: ducNuocData,
            borderColor: 'gray',
            borderWidth: 2,
        }]
    },
    options: { scales: { y: { beginAtZero: true } } }
});

// Cập nhật biểu đồ từ Firebase
firebase.database().ref('/sensor/Do_duc').on('value', (snap) => {
    const doDucValue = snap.val();
    const currentTime3 = new Date().toLocaleTimeString();

    // Thêm dữ liệu mới vào mảng
    ducNuocData.push(doDucValue);
    timeLabels3.push(currentTime3);

    // Giới hạn số lượng điểm dữ liệu
    if (ducNuocData.length > MAX_DATA_POINTS3) {
        ducNuocData.shift(); // Xóa phần tử đầu tiên
        timeLabels3.shift(); // Xóa nhãn thời gian đầu tiên
    }

    ducNuocChart.update();

    localStorage.setItem('ducNuocData', JSON.stringify(ducNuocData));
    localStorage.setItem('timeLabels3', JSON.stringify(timeLabels3));
});

//----------------------------------------------------------------------------------//

// Lấy phần tử canvas và khởi tạo biểu đồ
const ftx = document.getElementById('CDASChart').getContext('2d');
let ASData = JSON.parse(localStorage.getItem('ASData')) || [];
let timeLabels4 = JSON.parse(localStorage.getItem('timeLabels4')) || [];
const MAX_DATA_POINTS4 = 10; // Giới hạn số lượng điểm dữ liệu

const CDASChart = new Chart(ftx, {
    type: 'line',
    data: {
        labels: timeLabels4,
        datasets: [{
            label: 'Ánh sáng',
            data: ASData,
            borderColor: 'yellow',
            borderWidth: 2,
        }]
    },
    options: { scales: { y: { beginAtZero: true } } }
});

// Cập nhật biểu đồ từ Firebase
firebase.database().ref('/sensor/Cuong_do_as').on('value', (snap) => {
    const ASValue = snap.val();
    const currentTime4 = new Date().toLocaleTimeString();
    // Thêm dữ liệu mới vào mảng
    ASData.push(ASValue);
    timeLabels4.push(currentTime4);

    // Giới hạn số lượng điểm dữ liệu
    if (ASData.length > MAX_DATA_POINTS4) {
        ASData.shift(); // Xóa phần tử đầu tiên
        timeLabels4.shift(); // Xóa nhãn thời gian đầu tiên
    }

    CDASChart.update();

    localStorage.setItem('ASData', JSON.stringify(ASData));
    localStorage.setItem('timeLabels4', JSON.stringify(timeLabels4));
});

var lightTension = document.getElementById('cuongdoas');
var dbRef4 = firebase.database().ref('/sensor/Cuong_do_as');
dbRef4.on('value', snap => lightTension.innerText = snap.val()+" lux");

var mucNuoc = document.getElementById('mucnuoc');
var dbRef2 = firebase.database().ref('/sensor/Muc_nuoc');
dbRef2.on('value', snap => mucNuoc.innerText = snap.val()+" cm");

var doDuc = document.getElementById('doduc');
var dbRef3 = firebase.database().ref('/sensor/Do_duc');
dbRef3.on('value', snap => doDuc.innerText = snap.val()+"%");

var nhietDo = document.getElementById('nhietdo');
var dbRef = firebase.database().ref('/sensor/Nhiet_do');
dbRef.on('value', snap => nhietDo.innerText = snap.val()+"°C");