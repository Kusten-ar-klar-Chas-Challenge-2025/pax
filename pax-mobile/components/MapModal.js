import React from "react";
import {
    Modal,
    View,
    Image,
    TouchableOpacity,
    TouchableWithoutFeedback,
    Keyboard,
    StyleSheet,
    Text,
} from "react-native";

const MapModal = ({ isVisible, onClose, mapImage, imageDescription }) => {
    return (
        <Modal
            visible={isVisible}
            animationType="fade"
            transparent={true}
            onRequestClose={onClose}
        >
            <TouchableWithoutFeedback onPress={onClose}>
                <View style={styles.modalContainer}>
                    <TouchableWithoutFeedback>
                        <View style={styles.modalContent}>
                            <Image source={mapImage} style={styles.mapImage} />
                            {imageDescription && <Text style={styles.imageDescription}>{imageDescription}</Text>}
                        </View>
                    </TouchableWithoutFeedback>
                </View>
            </TouchableWithoutFeedback>
        </Modal>
    );
};

const styles = StyleSheet.create({
    modalContainer: {
        flex: 1,
        justifyContent: "center",
        alignItems: "center",
        backgroundColor: "rgba(0, 0, 0, 0.51)",
    },
    modalContent: {
        backgroundColor: "rgba(222, 247, 194, 0.99)",
        padding: 15,
        borderRadius: 10,
        alignItems: "center",
        width: "90%",
        maxHeight: "70%",
        marginTop: "1%",
    },
    mapImage: {
        maxWidth: "120%",
        height: 300,
        resizeMode: "contain",
    },
    imageDescription: {
        color: "FFF",
    },
});

export default MapModal;