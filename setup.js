(() => {
 const panel=document.getElementById('setup-panel');
 const chosen=document.getElementById('chosen-setup');
 const result=document.getElementById('setup-result');
 const tvResult=document.getElementById('tv-result');
 const devices={
  ios:{title:'Cài trên iPhone / iPad',steps:['Cài TestFlight từ App Store. Đây là app của Apple dùng để cài bản thử nghiệm.','Mở bot Telegram, bấm Start rồi chọn Bản iOS để nhận link. Mở link bằng TestFlight, bấm Chấp nhận rồi Cài đặt.'],action:'Nhận link cài iPhone / iPad',url:'https://t.me/consoletranslatorbot',guide:'guide.html#cai-app',note:'Cài xong, mở hướng dẫn có hình để bật Chơi từ xa và ghép máy lần đầu. iPhone và máy chơi game cần cùng mạng Wi-Fi.'},
  android:{title:'Cài trên điện thoại Android',steps:['Mở hướng dẫn Android, làm theo các bước cài app qua Google Play.','Cài và kết nối một app Chơi từ xa riêng theo hướng dẫn. Có hình game rồi mới mở Console Translator và chọn chia sẻ đúng app đó.'],action:'Xem các bước cài Android',url:'android-guide.html',guide:'android-guide.html',note:'Android cần phiên bản 11 trở lên. Console Translator đọc hình đang hiển thị, không tự kết nối với máy chơi game. Trang hướng dẫn có link app Chơi từ xa.'},
  mac:{title:'Cài trên máy Mac',steps:['Mở trang tải app Mac và tải file cài đặt. Máy cần macOS 15 trở lên.','Mở bot Telegram, chọn App Mac để nhận mã, rồi dán mã vào ứng dụng.'],action:'Mở trang tải app Mac',url:'guide.html#mac',guide:'guide.html#mac',note:'Nếu Mac hỏi có cho phép mở ứng dụng hay không, làm theo hướng dẫn kèm bản cài.'}
 };
 function link(label,url,className){const a=document.createElement('a');a.textContent=label;a.href=url;a.className=className;return a;}
 function showDevice(key){const d=devices[key];if(!d)return;
  panel.querySelectorAll('[data-device]').forEach(b=>b.setAttribute('aria-pressed',String(b.dataset.device===key)));
  result.replaceChildren();const heading=document.createElement('h3');heading.textContent=d.title;
  const list=document.createElement('ol');for(const text of d.steps){const li=document.createElement('li');li.textContent=text;list.append(li);}
  const actions=document.createElement('div');actions.className='setup-actions';actions.append(link(d.action,d.url,'button primary'),link('Hướng dẫn có hình →',d.guide,'text-link'));
  const note=document.createElement('p');note.className='setup-note';note.textContent=d.note;
  result.append(heading,list,actions,note);document.getElementById('setup-placeholder').hidden=true;chosen.hidden=false;
 }
 const tvs={android:['Android TV / Google TV','Cài app nhận phụ đề trên TV trước, rồi ghép với điện thoại hoặc Mac bằng mã 8 số.','guide.html#phu-de-tv','Cách cài Android TV'],lg:['TV LG','Kiểm tra TV trước khi mua gói. Có thể cài từ iPhone, Android hoặc máy tính theo hướng dẫn. Bản LG đang thử nghiệm, chưa kiểm tra hết các đời TV.','guide.html#lg-tv','Cách cài trên TV LG'],samsung:['TV Samsung','Kiểm tra đời TV rồi làm theo hướng dẫn bộ cài trên máy tính. Không phải TV Samsung nào cũng hỗ trợ.','guide.html#samsung-tv','Cách cài trên TV Samsung'],unknown:['Bạn chưa biết TV thuộc loại nào?','Mở bảng hướng dẫn kiểm tra hãng và đời TV. Nếu vẫn chưa rõ, gửi ảnh mã model TV vào nhóm hỗ trợ.','tv.html#kiểm-tra-tv-trước-khi-mua','Xem cách kiểm tra TV']};
 function showTV(key){panel.querySelectorAll('[data-tv]').forEach(b=>b.setAttribute('aria-pressed',String(b.dataset.tv===key)));tvResult.replaceChildren();const data=tvs[key];if(!data)return;const box=document.createElement('div');box.className='tv-next-step';const title=document.createElement('h3');title.textContent=data[0];const text=document.createElement('p');text.textContent=data[1];box.append(title,text,link(data[3]+' →',data[2],'button secondary'));tvResult.append(box);}
 panel.addEventListener('click',e=>{const device=e.target.closest('[data-device]');if(device)showDevice(device.dataset.device);const tv=e.target.closest('[data-tv]');if(tv)showTV(tv.dataset.tv);});
 panel.hidden=false;
})();
