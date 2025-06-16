import React, { useState, useEffect } from "react";
import {
  Modal,
  View,
  Text,
  Pressable,
  StyleSheet,
  ScrollView,
  TextInput,
  AccessibilityInfo,
} from "react-native";

const BookingModal = ({ visible, onClose, room, onBookingSuccess, theme }) => {
  const styles = createStyles(theme);

  const [name, setName] = useState("");
  const [date, setDate] = useState("2025-05-26");
  const [startTime, setStartTime] = useState("10:00");
  const [endTime, setEndTime] = useState("11:00");
  const [error, setError] = useState(null);
  const [success, setSuccess] = useState(false);
  const [bookings, setBookings] = useState([]);

  useEffect(() => {
    if (visible && room) {
      fetchBookings();
    }
  }, [visible, room, date]);

  const fetchBookings = async () => {
    try {
      const res = await fetch("https://paxdb.vercel.app/bookings");
      const data = await res.json();

      const filtered = data.filter((b) => {
        const bDate = new Date(b.start_time).toISOString().split("T")[0];
        return b.room_id === room.id && bDate === date;
      });

      setBookings(filtered);
    } catch (err) {
      console.error("Kunde inte hämta bokningar", err);
    }
  };

  const handleBooking = async () => {
    setError(null);

    if (!name || !date || !startTime || !endTime) {
      setError("Alla fält måste fyllas i.");
      return;
    }

    const [startHour, startMinute] = startTime.split(":").map(Number);
    const [endHour, endMinute] = endTime.split(":").map(Number);
    const [year, month, day] = date.split("-").map(Number);

    const requestedStart = new Date(
      Date.UTC(year, month - 1, day, startHour, startMinute)
    );
    const requestedEnd = new Date(
      Date.UTC(year, month - 1, day, endHour, endMinute)
    );

    if (requestedStart >= requestedEnd) {
      setError("Starttid måste vara före sluttid.");
      return;
    }

    for (let b of bookings) {
      const bStart = new Date(b.start_time);
      const bEnd = new Date(b.end_time);
      if (requestedStart < bEnd && bStart < requestedEnd) {
        setError("Tiden är redan bokad. Välj en annan.");
        return;
      }
    }

    try {
      const response = await fetch("https://paxdb.vercel.app/bookings", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          room_id: room.id,
          user_id: 1,
          start_time: requestedStart.toISOString(),
          end_time: requestedEnd.toISOString(),
        }),
      });

      if (!response.ok) throw new Error("Fel vid bokning");

      setSuccess(true);
      onBookingSuccess(room.id);
      AccessibilityInfo.announceForAccessibility("Bokning lyckades");

      setTimeout(() => {
        setSuccess(false);
        setName("");
        onClose();
      }, 2000);
    } catch (err) {
      console.error(err);
      setError("Det gick inte att boka rummet.");
    }
  };

  return (
    <Modal
      visible={visible}
      animationType="slide"
      transparent={true}
      accessible={true}
      accessibilityViewIsModal={true}
    >
      <View style={styles.modalBackground}>
        <View style={styles.modalContent}>
          <ScrollView
            accessible={true}
            accessibilityLabel={`Bokningsformulär för ${
              room?.name || "rummet"
            }`}
          >
            <Text style={styles.title}>Boka: {room?.name}</Text>

            <TextInput
              style={styles.input}
              placeholder="Ditt namn"
              value={name}
              onChangeText={setName}
              placeholderTextColor={theme.textSecondary}
              accessibilityLabel="Fält för namn"
            />

            <Text style={styles.label}>Datum (YYYY-MM-DD):</Text>
            <TextInput
              style={styles.input}
              placeholder="2025-05-26"
              value={date}
              onChangeText={setDate}
              placeholderTextColor={theme.textSecondary}
              accessibilityLabel="Fält för datum i formatet år-månad-dag"
            />

            <Text style={styles.label}>Starttid (HH:mm):</Text>
            <TextInput
              style={styles.input}
              value={startTime}
              onChangeText={setStartTime}
              placeholder="10:00"
              keyboardType="numeric"
              placeholderTextColor={theme.textSecondary}
              accessibilityLabel="Fält för starttid i formatet timmar och minuter"
            />

            <Text style={styles.label}>Sluttid (HH:mm):</Text>
            <TextInput
              style={styles.input}
              value={endTime}
              onChangeText={setEndTime}
              placeholder="11:00"
              keyboardType="numeric"
              placeholderTextColor={theme.textSecondary}
              accessibilityLabel="Fält för sluttid i formatet timmar och minuter"
            />

            {error && (
              <Text
                style={styles.error}
                accessibilityLiveRegion="polite"
                accessibilityRole="alert"
              >
                {error}
              </Text>
            )}

            {success && (
              <Text
                style={styles.success}
                accessibilityLiveRegion="polite"
                accessibilityRole="alert"
              >
                Bokning lyckades!
              </Text>
            )}

            <Pressable
              style={styles.button}
              onPress={handleBooking}
              accessibilityRole="button"
              accessibilityLabel="Bekräfta bokning"
              accessibilityHint="Tryck för att skicka bokningen"
            >
              <Text style={styles.buttonText}>Bekräfta bokning</Text>
            </Pressable>

            <Pressable
              style={[styles.button, { marginTop: 10 }]}
              onPress={onClose}
              accessibilityRole="button"
              accessibilityLabel="Avbryt"
              accessibilityHint="Tryck för att stänga formuläret utan att boka"
            >
              <Text style={styles.buttonText}>Avbryt</Text>
            </Pressable>
          </ScrollView>
        </View>
      </View>
    </Modal>
  );
};

const createStyles = (theme) =>
  StyleSheet.create({
    modalBackground: {
      flex: 1,
      backgroundColor: "rgba(0,0,0,0.5)",
      justifyContent: "center",
      alignItems: "center",
    },
    modalContent: {
      backgroundColor: theme.background,
      padding: 20,
      borderRadius: 20,
      width: "85%",
      maxHeight: "90%",
    },
    title: {
      fontSize: 18,
      fontWeight: "700",
      color: theme.textPrimary,
      marginBottom: 10,
    },
    label: {
      color: theme.textPrimary,
      marginTop: 10,
      marginBottom: 5,
    },
    input: {
      borderWidth: 1,
      borderColor: theme.textSecondary,
      borderRadius: 8,
      padding: 10,
      marginBottom: 10,
      color: theme.textPrimary,
    },
    button: {
      backgroundColor: "#7DBA6A",
      padding: 12,
      borderRadius: 8,
      alignItems: "center",
    },
    buttonText: {
      color: "#fff",
      fontWeight: "600",
    },
    error: {
      color: "red",
      textAlign: "center",
      marginBottom: 10,
    },
    success: {
      color: "green",
      textAlign: "center",
      marginBottom: 10,
    },
  });

export default BookingModal;
