# Network Monitoring and Anomaly Detection Project

## Proje Açıklaması
Bu proje, ağ üzerindeki cihazları tarayan, ağ trafiğini izleyen, sistem kaynaklarını raporlayan ve anomali/saldırı tespiti yapan bir C++ uygulamasıdır.
Uygulama, terminal arayüzü kullanarak kullanıcıya etkileşimli seçenekler sunar ve log dosyalarına tarama ve izleme sonuçlarını kaydeder.

## Gerekli Bağımlılıklar ve Kurulumu
Bu proje için aşağıdaki bağımlılıkların kurulması gerekmektedir:
- **build-essential**: C++ derleyici araçları
- **libpcap-dev**: Ağ trafiği yakalama kütüphanesi
- **libncurses5-dev**: Terminal tabanlı kullanıcı arayüzü kütüphanesi
- **libssl-dev**: SSL/TLS kütüphanesi
- **net-tools**: Ağ yapılandırma araçları (ifconfig dahil)

Bağımlılıkları kurmak için:
sudo apt-get update
sudo apt-get install -y build-essential libpcap-dev libncurses5-dev libssl-dev net-tools

## Projenin derlenmesi
Projeyi derlemek için projonenin olduğu dizinde make yazılabilir ve gerekli bağımlıkları da otomatik olarak kurabilirsin.
Makefile dosyası içinde otomatik bağımlıkları indirme mevcuttur.

## Projenin Temel İşlevleri
Bu proje aşağıdaki temel işlevleri sunar:

**Ağa Bağlı Cihazları Tara**:
Ağ üzerinde bağlı olan cihazları tarar ve log dosyasına kaydeder.
**Ağ Trafiğini İzle**:
Ağ trafiğini gerçek zamanlı olarak izler ve terminale paket bilgilerini yazdırır.
**Sunucu Ol**:
Bir SSL/TLS sunucusu başlatır ve istemcilerden gelen bağlantıları kabul eder.
**İstemci Ol**:
Bir SSL/TLS istemcisi olarak sunucuya bağlanır ve log dosyasını sunucudan indirir.
**Sistem Kaynaklarını İzle ve Raporla**:
CPU, bellek, disk ve ağ kullanımını izler ve raporlar.
**Anomali ve Saldırı Tespiti**:
Anomali ve saldırı kalıplarını tespit eder ve kullanıcıya bildirir, log dosyasına kaydeder.
