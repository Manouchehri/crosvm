#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

/// Represents an individual attached USB device.
struct UsbDeviceEntry {
  /// Internal port index used for identifying this individual device.
  uint8_t port;
  /// USB vendor ID
  uint16_t vendor_id;
  /// USB product ID
  uint16_t product_id;
};

/// Similar to internally used `BalloonStats` but using i64 instead of
/// Option<u64>. `None` (or values bigger than i64::max) will be encoded as -1.
struct BalloonStatsFfi {
  int64_t swap_in;
  int64_t swap_out;
  int64_t major_faults;
  int64_t minor_faults;
  int64_t free_memory;
  int64_t total_memory;
  int64_t available_memory;
  int64_t disk_caches;
  int64_t hugetlb_allocations;
  int64_t hugetlb_failures;
};

extern "C" {

/// Stops the crosvm instance whose control socket is listening on `socket_path`.
///
/// The function returns true on success or false if an error occured.
bool crosvm_client_stop_vm(const char *socket_path);

/// Suspends the crosvm instance whose control socket is listening on `socket_path`.
///
/// The function returns true on success or false if an error occured.
bool crosvm_client_suspend_vm(const char *socket_path);

/// Resumes the crosvm instance whose control socket is listening on `socket_path`.
///
/// The function returns true on success or false if an error occured.
bool crosvm_client_resume_vm(const char *socket_path);

/// Adjusts the balloon size of the crosvm instance whose control socket is
/// listening on `socket_path`.
///
/// The function returns true on success or false if an error occured.
bool crosvm_client_balloon_vms(const char *socket_path, uint64_t num_bytes);

/// Returns all USB devices passed through the crosvm instance whose control socket is listening on `socket_path`.
///
/// The function returns the amount of entries written.
/// # Arguments
///
/// * `socket_path` - Path to the crosvm control socket
/// * `entries` - Pointer to an array of `UsbDeviceEntry` where the details about the attached
///               devices will be written to
/// * `entries_length` - Amount of entries in the array specified by `entries`
///
/// Crosvm supports passing through up to 255 devices, so pasing an array with 255 entries will
/// guarantee to return all entries.
ssize_t crosvm_client_usb_list(const char *socket_path,
                               UsbDeviceEntry *entries,
                               ssize_t entries_length);

/// Attaches an USB device to crosvm instance whose control socket is listening on `socket_path`.
///
/// The function returns the amount of entries written.
/// # Arguments
///
/// * `socket_path` - Path to the crosvm control socket
/// * `bus` - USB device bus ID
/// * `addr` - USB device address
/// * `vid` - USB device vendor ID
/// * `pid` - USB device product ID
/// * `dev_path` - Path to the USB device (Most likely `/dev/bus/usb/<bus>/<addr>`).
/// * `out_port` - (optional) internal port will be written here if provided.
///
/// The function returns true on success or false if an error occured.
bool crosvm_client_usb_attach(const char *socket_path,
                              uint8_t bus,
                              uint8_t addr,
                              uint16_t vid,
                              uint16_t pid,
                              const char *dev_path,
                              uint8_t *out_port);

/// Detaches an USB device from crosvm instance whose control socket is listening on `socket_path`.
/// `port` determines device to be detached.
///
/// The function returns true on success or false if an error occured.
bool crosvm_client_usb_detach(const char *socket_path, uint8_t port);

/// Modifies the battery status of crosvm instance whose control socket is listening on
/// `socket_path`.
///
/// The function returns true on success or false if an error occured.
bool crosvm_client_modify_battery(const char *socket_path,
                                  const char *battery_type,
                                  const char *property,
                                  const char *target);

/// Resizes the disk of the crosvm instance whose control socket is listening on `socket_path`.
///
/// The function returns true on success or false if an error occured.
bool crosvm_client_resize_disk(const char *socket_path, uint64_t disk_index, uint64_t new_size);

/// Returns balloon stats of the crosvm instance whose control socket is listening on `socket_path`.
///
/// The parameters `stats` and `actual` are optional and will only be written to if they are
/// non-null.
///
/// The function returns true on success or false if an error occured.
///
/// # Note
///
/// Entries in `BalloonStatsFfi` that are not available will be set to `-1`.
bool crosvm_client_balloon_stats(const char *socket_path, BalloonStatsFfi *stats, uint64_t *actual);

} // extern "C"
