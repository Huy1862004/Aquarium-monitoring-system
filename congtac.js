const links = document.querySelectorAll('.slidebar a');

// Lặp qua từng liên kết và thêm sự kiện click
links.forEach(link => {
    link.addEventListener('click', function(event) {
        event.preventDefault(); // Ngăn chặn hành động mặc định là mở link trong tab mới

        const href = this.getAttribute('href');

        if (href && href !== '#') {
            // Thay đổi URL của trang mà không mở tab mới
            window.location.href = href;
        } 
    });
});
/*--------------------*/
//nút nhấn Nightshift
const nightShiftSwitch = document.getElementById('nightshift');
const backgroundElement = document.querySelector('.background'); 
const toggleIcon = document.getElementById('toggle-icon'); /*đổi icon*/

// Kiểm tra trạng thái checkbox từ localStorage khi tải trang
if (localStorage.getItem('nightShift') === 'true') {
    nightShiftSwitch.checked = true; 
    backgroundElement.style.backgroundImage = "url('./img/night_background.jpg')"; 
    toggleIcon.classList.remove('fa-sun'); 
    toggleIcon.classList.add('fa-moon'); 
} else {
    nightShiftSwitch.checked = false; 
    backgroundElement.style.backgroundImage = "url('./img/background.jpg')"; 
    toggleIcon.classList.remove('fa-moon'); 
    toggleIcon.classList.add('fa-sun'); 
}


// Lắng nghe sự kiện thay đổi khi nhấp vào công tắc
nightShiftSwitch.addEventListener('change', function() {
    if (this.checked) {
        backgroundElement.style.backgroundImage = "url('./img/night_background.jpg')";
        localStorage.setItem('nightShift', 'true'); 
    } else {
        // Nếu công tắc tắt (unchecked), đặt background là hình ban ngày
        backgroundElement.style.backgroundImage = "url('./img/background.jpg')";
        localStorage.setItem('nightShift', 'false');
    }
});

// Lắng nghe sự kiện thay đổi khi nhấp vào công tắc
nightShiftSwitch.addEventListener('change', function() {
    if (this.checked) {
        toggleIcon.classList.remove('fa-sun'); 
        toggleIcon.classList.add('fa-moon'); 
    } else {
        toggleIcon.classList.remove('fa-moon'); 
        toggleIcon.classList.add('fa-sun'); 
    }
});
//----------------------------------------------------------------------------------------------------------------------------------------------

// Lấy các nút và hình ảnh cho Feed
const onButtonFeed = document.querySelector('#on_btn'); // Lấy bằng querySelector
const offButtonFeed = document.querySelector('#off_btn'); // Lấy bằng querySelector
const feedImage = document.querySelector('#feed-image'); // Lấy hình ảnh

// Sự kiện khi nhấn nút On cho Feed
onButtonFeed.addEventListener('click', function() {
    console.log("Feed animation started.");
    maychoan.set(1); // Set giá trị lên Firebase
});

// Sự kiện khi nhấn nút Off cho Feed
offButtonFeed.addEventListener('click', function() {
    console.log("Feed animation stopped.");
    maychoan.set(0); // Set giá trị xuống Firebase
});

//----------------------------------------------------------------------------------------------------------------------------------------------

// Lấy các nút và hình ảnh cho Đèn
const onButtonLight = document.querySelector('#on_btn2'); // Lấy bằng querySelector
const offButtonLight = document.querySelector('#off_btn2'); // Lấy bằng querySelector
const lightImage = document.querySelector('#light-image'); // Lấy hình ảnh

// Sự kiện khi nhấn nút On cho Đèn
onButtonLight.addEventListener('click', function() {
    console.log("Đèn đã bật.");
    congtacden.set(1); // Set giá trị lên Firebase
});

// Sự kiện khi nhấn nút Off cho Đèn
offButtonLight.addEventListener('click', function() {
    console.log("Đèn đã tắt.");
    congtacden.set(0); // Set giá trị xuống Firebase
});


//----------------------------------------------------------------------------------------------------------------------------------------------

// Lấy các nút và hình ảnh cho Motor
const motorImage = document.querySelector('#motor-image'); // Lấy bằng querySelector
const hutButton = document.querySelector('#hut_btn'); // Lấy bằng querySelector
// const bomButton = document.querySelector('#bom_btn'); // Lấy bằng querySelector
const offButton = document.querySelector('#off_btn3'); // Lấy bằng querySelector

// Hàm để tắt động cơ sau 10 giây
function resetMotor() {
    hutButton.disabled = false; // Bật lại nút Hút
    // bomButton.disabled = false; // Bật lại nút Bơm
    console.log("Motor đã tắt.");
}

// Hàm xử lý sự kiện Hút hoặc Bơm
function startMotor() {
    hutButton.disabled = true; // Vô hiệu hóa nút Hút
    // bomButton.disabled = true; // Vô hiệu hóa nút Bơm
    console.log("Motor đang hoạt động.");

    // Tắt động cơ sau 10 giây và kích hoạt lại các nút
    setTimeout(resetMotor, 10000); // 10 giây
}

// Sự kiện khi nhấn nút Hút
hutButton.addEventListener('click', function() {
    startMotor();
    console.log("Hút nước đã bắt đầu.");
    maybom.set(1); // Set giá trị lên Firebase cho "Hút"
});

// Sự kiện khi nhấn nút Bơm
// bomButton.addEventListener('click', function() {
//     startMotor();
//     console.log("Bơm nước đã bắt đầu.");
//     maybom.set(1); // Set giá trị lên Firebase cho "Bơm"
// });

// Sự kiện khi nhấn nút Off
offButton.addEventListener('click', function() {
    resetMotor();
    console.log("Motor đã bị tắt thủ công.");
    maybom.set(0); // Set giá trị xuống Firebase
});
